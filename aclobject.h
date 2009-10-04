#ifndef MGG2102_SECURITY_ACLOBJECT_H
#define MGG2102_SECURITY_ACLOBJECT_H

#ifndef OBJECTSTORE_PATH
#define OBJECTSTORE_PATH "objectstore"
#endif

#include <memory>
#include <string>
// #include "object.h"
#include "user.h"
#include "userobject.h"
#include "object.h"
#include "permissionsobject.h"

namespace object_store
{
  using namespace std;
  
  class ACLObject : PermissionsObject
  {
    auto_ptr<PermissionsObject> _acl_object;
    auto_ptr<string> _group;
    auto_ptr<User> _user;
    short _permissions;
    static const short READ = 0x1;
    static const short WRITE = 0x2;
    static const short EXECUTE = 0x4;
    static const short VIEW = 0x8;
    static const short PERMISSIONS = 0x10;
    
  public:
    
    
    class ACLObjectException : public exception 
     {
       protected:
        auto_ptr<const string> _group;
        auto_ptr<const string> _user;
       public:
        ACLObjectException(const string& group, const string& user = "") throw();
        virtual ~ACLObjectException() throw();
        virtual const char* what() const throw();
     };
     
     /**
      validates group name.  1-254 chars, a-zA-Z0-9._
      @param groupname the group name to validate
      @return true if the groupname is valid
     */
     static bool valid_group(const string& groupname);
  
    /**
      creates / loads an acl object.
    */
    ACLObject(const string& user_name, const string& group_name, const string& owner_name, const string& object_name) throw(Object::ObjectException, User::UserException, UserObject::UserObjectException, ACLObjectException);
    
    ACLObject(const ACLObject& rhs);
    
    virtual Object* clone() const;
    
    
    Object* ACL();
    /**
      checks to see if the user can read/write/execute/view/write.
    */
    bool can_read();
    bool can_write();
    bool can_execute();
    bool can_view_permissions();
    bool can_write_permissions();
  };
}
#endif