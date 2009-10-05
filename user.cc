#include "object.h"
#include "user.h"
#include <sstream>

namespace object_store
{
  auto_ptr<Object> User::userobj(new Object("userfile", OBJECTSTORE_PATH));
  
  User::UserException::UserException(const string& user_name, bool doesnt_exist) throw() : _user_name(new string(user_name)), _doesnt_exist(doesnt_exist)
  {}
  User::UserException::~UserException() throw()
  {}
  const char* User::UserException::what() const throw()
  {
    string err_str = "UserException: ";
    if(_doesnt_exist)
      err_str += " User with user_name \""+ *_user_name +"\" doesn't exist";
    else
      err_str += " user_name \""+ *_user_name +"\" is invalid.  Usernames must be less than 254 chars and can only contain letters, numbers, underscores, and periods.";
      
    return err_str.c_str();
  }
  
  const vector<const User*>* User::users()
  {
    vector<const User*>* rval = new vector<const User*>();
    stringstream user_str;
    user_str << *User::userobj;
    
    string line;
    while(user_str.good())
    {
      getline(user_str, line);
      istringstream line_parser(line);
      if(line_parser.good())
      {
        string username;
        line_parser >> username;
        rval->push_back(new User(username));
      }
    }
    return rval;
  }
  
  bool User::set_userobj(Object* obj)
  {
    bool pass = false;
    //syntax checking
        
    stringstream user_str;
    user_str << *obj;
    
    string token;
    string line;
    
    while(user_str.good())
    {
      getline(user_str, line);
      if(line.length() == 0)
        return false;
      istringstream line_parser(line);
      while(line_parser.good())
      {
        line_parser >> token;
        if(User::valid_name(token))
          pass = true;
        else
          return false;
      }
    }
    
    if(pass)
    {
      stringstream asdf;
      asdf << *obj;
      asdf >> *User::userobj;
    }
    return pass;
  }
  
  
  
  bool User::valid_name(const string& user_name)
  {
    bool pass = Object::valid_name(user_name); //is a valid file?  (this is slow)
    if(pass)
    {
      string::const_iterator it;
  
      for ( it = user_name.begin() ; it < user_name.end(); it++ )
      {
        if(!( (*it >= 'a' && *it <= 'z') || //lower case?
              (*it >= 'A' && *it <= 'Z') || //upper case?
              (*it >= '0' && *it <= '9') || //numeral?
              *it == '.' || //period or
              *it == '_') ) //underscore?
          return false; //if not, return false. 
      }
    }
    return pass; 
  }
 
  User::User(const User& rhs)
          : _user_name(new string(*(rhs._user_name))), 
            _groups(new vector<const string*>)
  {
    vector<const string*>::iterator it;
    for(it = rhs._groups->begin(); it < rhs._groups->end(); it++)
      _groups->push_back(new string(**it));
  }
  
  User::User(const string& user_name) throw (UserException) : _user_name(new string(user_name)), _groups(new vector<const string*>)
  {
    bool fail = true;
    
    if(!valid_name(user_name))
    {
      UserException ue(user_name, false);
      throw ue;
    }
    
    stringstream user_str;
    
    user_str << *User::userobj;
    
    string user;
    string* group;
    string line;
    
    while(user_str.good() && fail) //while user/groups not set
    {
      getline(user_str, line);
      if(line.length() > 0)
      {
        istringstream line_parser(line);
        line_parser >> user;
        if(user == user_name)
        {
          fail = false;
          while(line_parser.good())
          {
            group = new string();
            line_parser >> *group;
            _groups->push_back(group);
          }
        }
      } 
    }
    
    if(fail)
    {
      UserException ue(user_name, true);
      throw ue;
    }      
  }
  
  User::~User()
  {
    vector<const string*>::iterator it;
    for(it = _groups->begin(); it < _groups->end(); it++)
      delete *it;
  }
  
  string User::name() const
  {
    return *_user_name;
  }
  
  
	/**
	  gets all of this users' groups
	  @return a pointer to a vector of string*'s, each is a group this user is in. 
	*/
	const vector<const string*>* User::groups() const
	{
    return _groups.get();
	}
	
	/**
	  checks to see if user is in the group
	  @return true if user is in the group, false otherwise
	*/
  bool User::in_group(const string& group_name) const
  {
    vector<const string*>::iterator it;
    for(it = _groups->begin(); it < _groups->end(); it++)
      if(**it == group_name)
        return true;
    return false;
  }
  
  
}
