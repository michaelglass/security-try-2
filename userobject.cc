#include "userobject.h"

#include "user.h"
#include "object.h"
#include <sys/types.h> //needed for various directory methods
#include <sys/stat.h> //needed for various directory methods.
#include <dirent.h>
#include <errno.h>
#include <string>

namespace object_store
{
  UserObject::UserObjectException::UserObjectException(const string& name) throw() : _name(new string(name))
  {}
  UserObject::UserObjectException::~UserObjectException() throw()
  {}
  const char* UserObject::UserObjectException::what() const throw()
  {
    return ("UserObjectException: UserObject name \""+ *_name +"\" is invalid.  UserObject names must be less than 254 chars and can only contain letters, numbers, underscores, and periods.").c_str();
  }
    
  string UserObject::path(const string& user_name)
  {
    return OBJECTSTORE_PATH "/^" + user_name;
  }
  
  bool UserObject::valid_name(const string& name)
  {
    bool pass = Object::valid_name(name); //is a valid file?  (this is slow)
    if(pass)
    {
      string::const_iterator it;
  
      for ( it = name.begin() ; it < name.end(); it++ )
      {
        if(!( (*it >= 'a' && *it <= 'z') || //lower case?
              (*it >= 'A' && *it <= 'Z') || //upper case?
              (*it >= '0' && *it <= '9') || //numeral?
              *it == '.' || //period or
              *it == '_' ||
              *it == '@') ) //underscore?
          return false; //if not, return false. 
      }
    }
    return pass; 
  }  
  
  bool UserObject::make_homedirs()
  {
    bool success = true;
    auto_ptr<const vector<const User*> > users(User::users());
    
    vector<const User*>::const_iterator it;
    struct stat s;
    
    for(it = users->begin(); it < users->end(); it++)
    {
      const char* path = UserObject::path((*it)->name()).c_str();
      delete *it;
      //check to see if the object storedirectory exists
      if(stat(path, &s) != 0)
        if(mkdir(path, S_IRWXU) != 0)
          success = false;
    }
    
    return success;
  }
  UserObject::UserObject(const string& user_name, const string& object_name) throw(Object::ObjectException, User::UserException, UserObjectException) : _owner(new User(user_name)), Object(object_name, UserObject::path(user_name))
  {
    if(!UserObject::valid_name(object_name))
    {
      UserObjectException uoe(object_name);
      throw uoe;
    }
  }
  
  UserObject::UserObject(const UserObject& rhs) : Object(rhs), _owner(new User(*(rhs._owner)))
  {}
 
  Object* UserObject::clone() const
  {
    return new UserObject(*this);
  }
  
  vector<UserObject*>* UserObject::objects(const string& user_name) throw(User::UserException)
  {
    User u(user_name); //throws error if there's an issue
    string path(UserObject::path(user_name));
    
    vector<UserObject*>* rval = new vector<UserObject*>();
 
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(path.c_str())) == NULL) {
      cerr << "Error(" << errno << ") opening " << path << endl;
      return 0;
    }

    while ((dirp = readdir(dp)) != NULL)
    {
      string objname(dirp->d_name);
      if(objname == "." || objname == "..")
        continue;
      else
        objname = objname.substr(1); //first char is prefix
      rval->push_back(new UserObject(user_name, objname));
    }

    closedir(dp);
    return rval;    
  }
  
  
 
}