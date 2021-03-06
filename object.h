#ifndef MGG2102_SECURITY_OBJECT_H
#define MGG2102_SECURITY_OBJECT_H

#include <memory>
#include <iostream>
// #include <exception>

namespace object_store
{
  using namespace std;
  
  class Object
  {
  protected:
    
    auto_ptr<string> _name;
    auto_ptr<string> _path;
    
    virtual ostream& write(ostream& os) throw(exception);
    virtual istream& read(istream& is) throw(exception);
    
  public:
    class ObjectException : public exception 
    {
      protected:
       auto_ptr<const string> _name;
       auto_ptr<const string> _path;

      public:
       ObjectException(const string& name, const string& path = "") throw();
       ObjectException(const ObjectException& rhs) throw();
       virtual ~ObjectException() throw();
       virtual const char* what() const throw();
    };
    
    /**
      name has to be at least one char long and less than 255 chars long (or stdlib's FILENAME_MAX, whichever is smaller)
      can only contains letters, digits, periods, underscores, @s and ^s.
    */
    static bool valid_name(const string& name);
    
    /**
      path has to be a pre-existing directory.  it can also have no two periods in a row and no initial / (must be relative path ).
    */
    static bool valid_path(const string& path);
    
    /**
      Creates an object.  Either opens the file at path/name or creates it
      throws ObjectException if path/name is malformed (doesn't pass the valid_name or valid_path check).
      
      @param const& name object's name
      @param const& path object's path
      @throw ObjectException if path/name is malformed
      @return a new object_store
    */
    Object(const string& name, const string& path = "") throw(ObjectException);//throw(ObjectException);
    
    /**
      copy constructor
    */
    Object(const Object &rhs);
    
    virtual Object* clone() const;
    /**
      getter for obj's name
    */
    string name() const;
    /**
      getter for obj's path
    */
    string path() const;
    
    /**
      gets whether or not this object is written
      @return whether object is saved
    */
    bool exists() const;
    
    /**
      gets the length of objects that exist.  returns 0 if they dont
      @return the size of the object
    */
    unsigned long length() const;
            
    /**
      used to output object's contents into an ostream
    */
    friend ostream& operator<< (std::ostream& os, object_store::Object& object);
    /**
      used to put data into an object
    */
    friend istream& operator>> (std::istream& is, object_store::Object& object);
    

  };
}

#endif
