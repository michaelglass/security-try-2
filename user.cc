#include "object.h"
#include "user.h"
#include <sstream>

namespace object_store
{
  auto_ptr<Object> User::userobj(new Object("userfile", "objectstore"));
  
  User::UserException::UserException(const string& username, bool doesnt_exist) throw() : _username(new string(username)), _doesnt_exist(doesnt_exist)
  {}
  User::UserException::~UserException() throw()
  {}
  const char* User::UserException::what() const throw()
  {
    string err_str = "UserException: ";
    if(_doesnt_exist)
      err_str += " User with username \""+ *_username +"\" doesn't exist";
    else
      err_str += " username \""+ *_username +"\" is invalid.  Usernames must be less than 255 chars and can only contain letters, numbers, underscores, and periods.";
      
    return err_str.c_str();
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
  
  
  
  bool User::valid_name(const string& username)
  {
    string::const_iterator it;
  
    for ( it = username.begin() ; it < username.end(); it++ )
    {
      if(!( (*it >= 'a' && *it <= 'z') || //lower case?
            (*it >= 'A' && *it <= 'Z') || //upper case?
            (*it >= '0' && *it <= '9') || //numeral?
            *it == '.' || //period or
            *it == '_') ) //underscore?
        return false; //if not, return false. 
    }
    return Object::valid_name(username); // I know this is slow
  }
  
  User::User(const string& username) throw (UserException) : _username(new string(username)), _groups(new vector<const string*>)
  {
    bool fail = true;
    
    if(!valid_name(username))
    {
      UserException ue(username, false);
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
        if(user == username)
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
      UserException ue(username, true);
      throw ue;
    }      
  }
  
  string User::name() const
  {
    return *_username;
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