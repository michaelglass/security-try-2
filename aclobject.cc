#include "aclobject.h"
#include <sstream>
#include <sys/types.h> //needed for various directory methods
#include <sys/stat.h> //needed for various directory methods.
#include <dirent.h>
#include <errno.h>

namespace object_store
{
  ACLObject::ACLObjectException::ACLObjectException(const string& group, const string& user, bool file_not_there) throw() : _group(new string(group)), _user(new string(user)), _file_not_there(file_not_there), _invalid_objname(false)
  {}
  ACLObject::ACLObjectException::ACLObjectException(const string& objname, bool invalid_objname) throw() : _group(new string(objname)), _invalid_objname(invalid_objname)
  {}
  ACLObject::ACLObjectException::~ACLObjectException() throw()
  {}
  const char* ACLObject::ACLObjectException::what() const throw()
  {
    if(_invalid_objname)
    {
      return ("ACLObjectException: Object name \"" + *_group + "\" is not valid.  Object names must be less than 254 chars and can only contain letters, numbers, underscores, and periods.").c_str();
    }
    if(_file_not_there)
      return "ACLObjectException: Object does not yet exist.  Can't access other users objects that they haven't created";
    
    return ("ACLObjectException: Groupname \""+*_group+"\" " + 
                              (_user->length() > 0 ? 
                                "does not exist for user with username \""+*_user+"\""  :
                              "is not valid.  Groupnames must be less than 254 chars and can only contain letters, numbers, underscores, and periods.") ).c_str();
  }


  ACLObject::ACLObject(const string& user_name, const string& group_name, const string& owner_name, const string& object_name) throw(Object::ObjectException, User::UserException, UserObject::UserObjectException, ACLObjectException) 
              : PermissionsObject(UserObject(owner_name, object_name) , true, true) , 
                _groups(new vector<const string*>) ,
                _user(new User(user_name)),
                _permissions(0)
  {
    _groups->push_back(new string(group_name));
    initialize(user_name, owner_name, object_name);
  }
  

  ACLObject::ACLObject(const string& user_name, vector<const string*>* groups, const string& owner_name, const string& object_name) throw(Object::ObjectException, User::UserException, UserObject::UserObjectException, ACLObjectException)
              : PermissionsObject(UserObject(owner_name, object_name) , true, true) , 
                _groups(groups) ,
                _user(new User(user_name)),
                _permissions(0)
  {
    initialize(user_name, owner_name, object_name);
  }

  
  
    

  void ACLObject::initialize(const string& user_name, const string& owner_name, const string& object_name) throw(Object::ObjectException, User::UserException, UserObject::UserObjectException, ACLObjectException)
  {    
    if(!valid_group(object_name))
    {
      ACLObjectException aoe(object_name, true);
      throw aoe;
    }
    // if(!valid_group(group_name))
    // {
    //   ACLObjectException aoe(group_name);
    //   throw aoe;
    // }
    // if(! _permissions->in_group(group_name) )
    // {
    //   ACLObjectException aoe(group_name, user_name);
    //   throw aoe;
    // }
    //else, grab permissions
    //couple of cases.  user == owner?
    if(!exists())
    {
      if(user_name != owner_name && !exists())
      {
        ACLObjectException aoe("","", true);
        throw aoe;
      }
      else
      {
        _permissions = READ | WRITE | EXECUTE | PERMISSIONS | VIEW;
        _acl_object = auto_ptr<ACL>(new ACL(UserObject(owner_name, object_name + "@"), true, true));
        return;
      }
    }
    else
    {
      auto_ptr<UserObject> acl(new UserObject(owner_name, object_name + "@"));
      stringstream acl_str;
      acl_str << *acl;
      string line;
      string user;
      string group;
      bool found_user_and_group = false;
      while(acl_str.good() && !found_user_and_group)
      {
        string line;
        getline(acl_str, line);
        if(line.length() > 0)
        {
          size_t curr_offset = 0;
          size_t next_offset = line.find('.', curr_offset);
          string user = line.substr(curr_offset, next_offset-curr_offset);
          if(user == "*" || user == user_name) //username match!
          {
            curr_offset = next_offset + 1; //skipping '.'
            next_offset = line.find(' ', curr_offset); 
            string group = line.substr(curr_offset, next_offset - curr_offset);
            bool group_match = (group == "*");
            vector<const string*>::const_iterator it;
            
            for(it = _groups->begin(); !group_match && it < _groups->end(); it++)
              group_match = **it == group;
            
            if(group_match) //group match!
            {
              found_user_and_group = true;
              string permissions = line.substr(next_offset+1);
              string::iterator it;
              for(it = permissions.begin(); it < permissions.end(); it++)
                switch(*it)
                {
                  case 'r':
                    _permissions |= READ;
                    break;
                  case 'w':
                    _permissions |= WRITE;
                    break;
                  case 'x':
                    _permissions |= EXECUTE;
                    break;
                  case 'p':
                    _permissions |= PERMISSIONS;
                    break;
                  case 'v':
                    _permissions |= VIEW;
                    break;
                  break;
                  case ' ':
                    //error, should not get here, but who cares
                  break;
                  default:
                    //error, should not get here, but who cares
                  break;
                }
            }
          }
        }
      }
      _can_read  = _permissions & READ;
      _can_write = _permissions & WRITE;
      _acl_object = auto_ptr<ACL>(new ACL(*acl, can_view_permissions(), can_write_permissions()));
    }
  }
  
  ACLObject::ACLObject(const ACLObject& rhs)
              : PermissionsObject(rhs), 
                _acl_object( (ACL*) rhs._acl_object->clone()), 
                _groups(new vector<const string*> ),
                _user(new User(*(rhs._user))),
                _permissions(rhs._permissions)
  {
    vector<const string*>::const_iterator it;
    for(it = rhs._groups->begin(); it < rhs._groups->end(); it++)
      _groups->push_back(new string(**it));
  }
  
  Object* ACLObject::clone() const
  {
    return new ACLObject(*this);
  }

  bool ACLObject::valid_group(const string& groupname)
  {
    return User::valid_name(groupname);
  }
  
  PermissionsObject* ACLObject::get_ACL() { return (PermissionsObject*)_acl_object->clone(); }
  
  bool ACLObject::can_execute() { return _permissions & EXECUTE; }
  bool ACLObject::can_view_permissions() { return _permissions & VIEW; }
  bool ACLObject::can_write_permissions() { return _permissions & PERMISSIONS; }
  
  istream& ACLObject::read(istream& is) throw()
  {
    if(!exists())
    {
      stringstream default_permissions;
      default_permissions << _user->name() << ".* rwxpv";
      default_permissions >> *_acl_object;
    }
    return PermissionsObject::read(is);
  }
  
  vector<UserObject*>* ACLObject::objects(const string& user_name) throw(User::UserException)
  {
    vector<UserObject*>* rval = UserObject::objects(user_name);
    if(!rval)
    {
      return new vector<UserObject*>;
    }
    vector<UserObject*>::iterator it;
    for(it = rval->begin(); it < rval->end(); it++)
    {
      UserObject* obj = *it;
      string objname = obj->name();
      if(objname[objname.length()-1] == '@')
      {
        delete *it;
        rval->erase(it);
      }
    }
    return rval;
  }
  
}
