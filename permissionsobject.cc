#include "permissionsobject.h"
#include <iostream>

namespace object_store
{
  PermissionsObject::PermissionsObjectException::PermissionsObjectException(const string& objname, bool can_read) throw() : _name(new string(objname)), _can_read(can_read)
  {}
  PermissionsObject::PermissionsObjectException::~PermissionsObjectException() throw()
  {}
  const char* PermissionsObject::PermissionsObjectException::what() const throw()
  {
    return ("PermissionsObjectException: Object \""+*_name+"\" cannot be " + (_can_read ? " written to " : " read from")).c_str();
  }
  
  ostream& PermissionsObject::write(ostream& os) throw(PermissionsObjectException)
  {
    if(can_read())
      return Object::write(os);
    else
    {
      PermissionsObjectException poe(*_name, false);
      throw poe;
    }
  }

  istream& PermissionsObject::read(istream& is) throw(PermissionsObjectException)
  {
    if(can_write())
      return Object::read(is);
    else
    {
      PermissionsObjectException poe(*_name, true);
      throw poe;
    }
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
