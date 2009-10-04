#include "aclobject.h"

namespace object_store
{
  ACLObject::ACLObjectException::ACLObjectException(const string& group, const string& user) throw() : _group(new string(group)), _user(new string(user))
  {}
  ACLObject::ACLObjectException::~ACLObjectException() throw()
  {}
  const char* ACLObject::ACLObjectException::what() const throw()
  {
    return ("ACLObjectException: Groupname \""+*_group+"\" " + 
                              (_user->length() > 0 ? 
                                "does not eist for user with username \""+*_user+"\""  :
                              "is not valid.  Groupnames must be less than 254 chars and can only contain letters, numbers, underscores, and periods.") ).c_str();
  }

  ACLObject::ACLObject(const string& user_name, const string& group_name, const string& owner_name, const string& object_name) throw(Object::ObjectException, User::UserException, UserObject::UserObjectException, ACLObjectException) 
              : PermissionsObject(UserObject(owner_name, object_name) , false, false) , 
                _acl_object(new PermissionsObject((UserObject(owner_name, object_name + "@")),false,false)),
                _group(new string(group_name)),
                _user(new User(user_name)),
                _permissions(0)
  {}
  
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

  static bool valid_group(const string& groupname)
  {
    return UserObject::valid_name(groupname);
  }
  
  Object* ACLObject::ACL() { return _acl_object.get(); }
  
  bool ACLObject::can_read() { return _permissions & READ; }
  bool ACLObject::can_write() { return _permissions & WRITE; }
  bool ACLObject::can_execute() { return _permissions & EXECUTE; }
  bool ACLObject::can_view_permissions() { return _permissions & VIEW; }
  bool ACLObject::can_write_permissions() { return _permissions & PERMISSIONS; }
}