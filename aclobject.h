#ifndef MGG2102_SECURITY_ACLOBJECT_H
#define MGG2102_SECURITY_ACLOBJECT_H

#ifndef OBJECTSTORE_PATH
#define OBJECTSTORE_PATH "objectstore"
#endif

#include <memory>
#include <string>
// #include "object.h"
#include "user.h"
#include "object.h"

namespace object_store
{
  using namespace std;
  
  class ACLObject
  {
    auto_ptr<PermissionsObject> _acl_object;
    auto_ptr<string> _group;
    auto_ptr<User> _user;
    int permissions;
  public:
    class ACLObjectException : public exception 
     {
       protected:
        auto_ptr<const string> _name;

       public:
        ACLObjectException(const string& name) throw();
        virtual ~ACLObjectException() throw();
        virtual const char* what() const throw();
     };
  
    /**
      creates / loads an acl object.
    */
    ACLObject(const string& user_name, const string& group_name, const string& owner_name, const string& object_name) throw(Object::ObjectException, User::UserException, UserObject::UserObjectException, ACLObjectException);

    /**
      checks to see if the user can read/write/execute/view/write.
    */
    
    string group();
    bool can_read();
    bool can_write();
    bool can_execute();
    bool can_view_permissions();
    bool can_write_permissions();
  };
}
#endif