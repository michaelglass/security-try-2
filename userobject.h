#ifndef MGG2102_SECURITY_USEROBJECT_H
#define MGG2102_SECURITY_USEROBJECT_H

#ifndef OBJECTSTORE_PATH
#define OBJECTSTORE_PATH "objectstore"
#endif

#include <memory>
#include <string>
#include <vector>
// #include "object.h"
#include "user.h"
#include "object.h"

namespace object_store
{
  using namespace std;
  
  class UserObject : public Object
  {
    auto_ptr<User> _owner;
    static string path(const string& user_name);
    
  public:
    /**
      returns a list of objs for that user
    */
    static vector<UserObject*>* objects(const string& user_name) throw(User::UserException);
    static bool valid_name(const string& name);
    
    class UserObjectException : public exception 
    {
      protected:
       auto_ptr<const string> _name;

      public:
       UserObjectException(const string& name) throw();
       virtual ~UserObjectException() throw();
       virtual const char* what() const throw();
    };
    /**
      creates all the homedirs for current users if they don't exist.  NEED TO RUN THIS BEFORE YOU USE USER_OBJECT
    */
    static bool make_homedirs();
    /**
      creates / loads a user object.  
    */
    UserObject(const string& user_name, const string& object_name) throw(Object::ObjectException, User::UserException, UserObjectException);
    UserObject(const UserObject& rhs);
    
    virtual Object* clone() const;
  };
}
#endif
