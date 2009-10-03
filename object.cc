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
    size_t max_length = FILENAME_MAX < 255 ? FILENAME_MAX : 255;
  
    size_t length = name.length();
    if(length < min_length || length > max_length || name == "." || name == "..") //too short, too long, "." or ".."
      return false;
    
    string::const_iterator it;
  
    for ( it = name.begin() ; it < name.end(); it++ )
    {
      if(!( (*it >= 'a' && *it <= 'z') || //lower case?
            (*it >= 'A' && *it <= 'Z') || //upper case?
            (*it >= '0' && *it <= '9') || //numeral?
            *it == '.' || //period or
            *it == '_' || //underscore?
            *it == '@' || //@ sign
            *it == '^' ) ) // or ^
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
    bool valid_name = Object::valid_name(name);
    bool valid_path = Object::valid_path(path);
  
    if(!valid_name || !valid_path) //FAIL!
    {
      ObjectException oe(valid_name? "": name, valid_path? "": path);
      throw oe;
    }
  }

  ostream& operator<< (std::ostream& os, Object& object)
  {
    if(object.exists())
    {
      ifstream obj;
      obj.open( object.path().c_str() );
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

  istream& operator>> (std::istream& is, Object& object)
  {
    ofstream obj;
    obj.open( object.path().c_str() );
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
  


  bool Object::exists() const
  {
    struct stat s;
    //if the file already exists . . . 
    return (  stat(path().c_str(), &s) == 0 &&      //path has to exist
                S_ISREG(s.st_mode) );               //and has to be a dir
  }

  string Object::name() const
  {  return *_name;  }

  string Object::path() const
  {  return *_path + (_path->length() == 0 ? "" : "/" ) + *_name;  }
  
}