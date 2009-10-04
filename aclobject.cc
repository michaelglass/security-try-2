#include "aclobject.h"
#include <sstream>

namespace object_store
{
  ACLObject::ACLObjectException::ACLObjectException(const string& group, const string& user, bool file_not_there) throw() : _group(new string(group)), _user(new string(user)), _file_not_there(file_not_there)
  {}
  ACLObject::ACLObjectException::~ACLObjectException() throw()
  {}
  const char* ACLObject::ACLObjectException::what() const throw()
  {
    if(_file_not_there)
      return "ACLObjectException: File does not yet exist.  Can't access other users files that they haven't created";
    
    return ("ACLObjectException: Groupname \""+*_group+"\" " + 
                              (_user->length() > 0 ? 
                                "does not eist for user with username \""+*_user+"\""  :
                              "is not valid.  Groupnames must be less than 254 chars and can only contain letters, numbers, underscores, and periods.") ).c_str();
  }

  ACLObject::ACLObject(const string& user_name, const string& group_name, const string& owner_name, const string& object_name) throw(Object::ObjectException, User::UserException, UserObject::UserObjectException, ACLObjectException) 
              : PermissionsObject(UserObject(owner_name, object_name) , false, false) , 
                _group(new string(group_name)),
                _user(new User(user_name))
  {
    if(!valid_group(group_name))
      throw new ACLObjectException(group_name);
    if(! _user->in_group(group_name) )
      throw new ACLObjectException(group_name, user_name);
    //else, grab permissions
    //couple of cases.  user == owner?
    if(!exists())
    {
      if(user_name != owner_name && !exists())
        throw new ACLObjectException("","", true);
      else
      {
        _permissions = READ | WRITE | EXECUTE | PERMISSIONS | VIEW;
        _acl_object = auto_ptr<PermissionsObject>(new PermissionsObject(UserObject(user_name, object_name + "@"), true, true));
        return;
      } 
    }
    else
    {
      auto_ptr<UserObject> acl(new UserObject(user_name, object_name + "@"));
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
            if(group == "*" || group == group_name) //group match!
            {
              found_user_and_group = true;
              string permissions = line.substr(next_offset+1);
              string::iterator it;
              for(it = permissions.begin(); it < permissions.end(); it++)
                switch(*it)
                {
                  case 'r':
                    _permissions |= READ;
                    _can_read = true;
                    break;
                  case 'w':
                    _permissions |= WRITE;
                    _can_write = true;
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
                
              _acl_object = auto_ptr<PermissionsObject>(new PermissionsObject(*acl, can_view_permissions(), can_write_permissions()));
            }
          }
        }

      }
    }
  }
  
  ACLObject::ACLObject(const ACLObject& rhs)
              : PermissionsObject(rhs), 
                _acl_object( (PermissionsObject*) rhs._acl_object->clone()), 
                _group(new string(*(rhs._group))),
                _user(new User(*(rhs._user))),
                _permissions(rhs._permissions)
  {}
  
  Object* ACLObject::clone() const
  {
    return new ACLObject(*this);
  }

  bool ACLObject::valid_group(const string& groupname)
  {
    return UserObject::valid_name(groupname);
  }
  
  Object* ACLObject::ACL() { return _acl_object.get(); }
  
  bool ACLObject::can_execute() { return _permissions & EXECUTE; }
  bool ACLObject::can_view_permissions() { return _permissions & VIEW; }
  bool ACLObject::can_write_permissions() { return _permissions & PERMISSIONS; }
}