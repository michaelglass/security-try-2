#include "object.h"
#include <sys/types.h> //needed for various directory methods
#include <sys/stat.h> //needed for various directory methods.
#include <fstream>
#include <memory>
#include <iostream>
#include <exception>


namespace object_store
{
  Object::ObjectException::ObjectException(const string& name, const string& path) throw() : _name(new string(name)), _path(new string(path))
  {}
  Object::ObjectException::ObjectException(const ObjectException& rhs) throw() : _name(new string(*rhs._name)), _path(new string(*rhs._path))
  {}
  

  Object::ObjectException::~ObjectException() throw()
  {}

  const char* Object::ObjectException::what() const throw()
  { 
    bool name = _name->length() > 0;
    bool path = _path->length() > 0;
    bool both = name && path;
    return ( "Object Exception: " +
             (name ? "invalid name \""+*_name+"\"" : "") +
             (both ? " and " : "") + 
             (path ? "invalid path \""+*_path+"\"" : "" )).c_str();
  }

  bool Object::valid_name(const string& name)
  {
    size_t min_length = 1;
    size_t max_length = (FILENAME_MAX < 255 ? FILENAME_MAX : 255) -1; //filename + 1-char prefix
  
    size_t length = name.length();
    if(length < min_length || length > max_length) //too short, too long, "." or ".."
      return false;
    
    string::const_iterator it;
  
    for ( it = name.begin() ; it < name.end(); it++ )
    {
      if(!( (*it >= 'a' && *it <= 'z') || //lower case?
            (*it >= 'A' && *it <= 'Z') || //upper case?
            (*it >= '0' && *it <= '9') || //numeral?
            *it == '.' || //period or
            *it == '_' || //underscore?
            *it == '@' ||
            *it == '=' ||
            *it == '+' ||
            *it == '{' ||
            *it == '}' ||
            *it == '[' ||
            *it == ']') ) //@ sign
        return false; //if not, return false. 
    }
    return true; // GREAT SUCCESS!
  }
  
  bool Object::valid_path(const string& path)
  {
    if(path.length() > 1) //if it's not empty...
    {
      struct stat s;
      if( path.find("..") != string::npos ||  //path has a .., illegal
          path[0] == '/' ||                   //path[0] can't be /
          stat(path.c_str(), &s) != 0 ||      //path has to exist
          !S_ISDIR(s.st_mode) )               //and has to be a dir
        return false;    
    }
    return true;
  }
  //ObjectException
  Object::Object(const string& name, const string& path) throw(ObjectException) : _name(new string(name)), _path(new string(path))
  {
    bool valid_name = this->valid_name(name);
    bool valid_path = Object::valid_path(path);
  
    if(!valid_name || !valid_path) //FAIL!
    {
      throw ObjectException(valid_name? "": name, valid_path? "": path);
    }
  }
  
  Object::Object(const Object &rhs) : _name(new string( *(rhs._name) )), _path( new string( *(rhs._path) ) )
  {}
  
  Object* Object::clone() const
  {
    return new Object(*this);
  }
  

  ostream& Object::write(ostream& os) throw(exception)
  {
    if(exists())
     {
       ifstream obj;
       obj.open( path().c_str() );
       if(obj.is_open())
       {
         char buff[4096]; //arbitrary 4k buffer
         streamsize size;
         while(obj.good())
         {
           obj.read(buff, 4096);
           size = obj.gcount();
           os.write(buff, size);
         }  
       }
       obj.close();
     }
     return os;
  }
  
  istream& Object::read(istream& is) throw(exception)
  {
    ofstream obj;
    obj.open( path().c_str() );
    if(obj.is_open())
    {
      char buff[4096];
      streamsize size;
      while(is.good())
      {
        is.read(buff, 4096);
        size = is.gcount();
        obj.write(buff, size);
      }    
    }
    obj.close();
    return is;
    
  }

  

  ostream& operator<< (std::ostream& os, Object& object)
  {
    return object.write(os);
  }

  

  istream& operator>> (std::istream& is, Object& object)
  {
    return object.read(is);
  }
  

  bool Object::exists() const
  {
    struct stat s;
    //if the file already exists . . . 
    return (  stat(path().c_str(), &s) == 0 &&      //path has to exist
                S_ISREG(s.st_mode) );               //and has to be a dir
  }
  
  unsigned long Object::length() const
  {
    if(!exists())
      return 0;
    unsigned long length;
    ifstream obj;
    obj.open( path().c_str() );
    obj.seekg (0, ios::end);
    length = obj.tellg();
    obj.close();
    return length;
  }

  string Object::name() const
  {  return *_name;  }

  string Object::path() const
  {  return *_path + (_path->length() == 0 ? "" : "/" ) + "%" + *_name;  }
  
}
