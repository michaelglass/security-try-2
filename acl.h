#ifndef MGG2102_SECURITY_ACL_H
#define MGG2102_SECURITY_ACL_H

#ifndef OBJECTSTORE_PATH
#define OBJECTSTORE_PATH "objectstore"
#endif

// #include "object.h"
#include "user.h"
#include "object.h"
#include "permissionsobject.h"

namespace object_store
{
  using namespace std;
  
  class ACL : public PermissionsObject
  {
  protected:
    virtual istream& read(istream& is);
  public:
    /**
      creates / loads an acl object.
    */
    ACL(const Object& obj, bool can_read = true, bool can_write = true);
    ACL(const ACL& rhs);

    virtual Object* clone() const;
  };
}
#endif