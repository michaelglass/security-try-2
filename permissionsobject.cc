#include "permissionsobject.h"
#include <iostream>

namespace object_store
{
  ostream& PermissionsObject::write(ostream& os)
  {
    if(can_read())
      return Object::write(os);
    else
      return os;
  }

  istream& PermissionsObject::read(istream& is)
  {
    if(can_write())
      return Object::read(is);
    else
      return is;
  }
  PermissionsObject::PermissionsObject(Object &obj, bool can_read, bool can_write) : Object(obj), _can_read(can_read), _can_write(can_write)
  {}

  bool PermissionsObject::can_read() { return _can_read;}
  bool PermissionsObject::can_write() { return _can_write; }
}