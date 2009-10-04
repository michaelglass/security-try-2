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
  PermissionsObject::PermissionsObject(const Object &obj, bool can_read, bool can_write) : Object(obj), _can_read(can_read), _can_write(can_write), _original(obj.clone())
  {}
  
  PermissionsObject::PermissionsObject(const PermissionsObject& rhs) : Object(rhs), _can_read(rhs._can_read), _can_write(rhs._can_write), _original(rhs._original->clone())
  {}
  
  Object* PermissionsObject::clone() const
  {
    return new PermissionsObject(*this);
  }
  
  Object* PermissionsObject::original()
  {
    return _original.get();
  }

  bool PermissionsObject::can_read() { return _can_read;}
  bool PermissionsObject::can_write() { return _can_write; }
}