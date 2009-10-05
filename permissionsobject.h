#ifndef MGG2102_SECURITY_PERMISSIONSOBJECT_H
#define MGG2102_SECURITY_PERMISSIONSOBJECT_H

#ifndef OBJECTSTORE_PATH
#define OBJECTSTORE_PATH "objectstore"
#endif

// #include "object.h"
#include "user.h"
#include "object.h"

namespace object_store
{
  using namespace std;
  
  class PermissionsObject : public Object
  {
  protected:
    auto_ptr<Object> _original;
    bool _can_read;
    bool _can_write;
    
  public:
    
    class PermissionsObjectException : public exception 
    {
      protected:
        auto_ptr<const string> _name;
        bool _can_read;
      public:
        PermissionsObjectException(const string& objname, bool can_read) throw();
        virtual ~PermissionsObjectException() throw();
        virtual const char* what() const throw();
    };
    /**
      creates / loads an acl object.
    */
    PermissionsObject(const Object& obj, bool can_read = true, bool can_write = true);
    PermissionsObject(const PermissionsObject& rhs);

    virtual Object* clone() const;

    Object* original();
    /**
      checks to see if the user can read this file.
    */
    virtual bool can_read();
    virtual bool can_write();
  protected:
    virtual ostream& write(ostream& os) throw(PermissionsObjectException);
    virtual istream& read(istream& is) throw(PermissionsObjectException);
  };
}
#endif
