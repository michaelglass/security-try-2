#ifndef MGG2102_SECURITY_USER_H
#define MGG2102_SECURITY_USER_H

#include <string>
#include <vector>
#include <memory>

namespace object_store
{
  class Object;
  
  using namespace std;
  class User
  {
    auto_ptr<string> _username;
    auto_ptr< vector<const string*> > _groups;
    
    static auto_ptr<Object> userobj;
  public:
        
    class UserException : public exception
    {
      bool _doesnt_exist;
      auto_ptr<string> _username;
      
      public:
       UserException(const string& username, bool doesnt_exist) throw();
       virtual ~UserException() throw();
       virtual const char* what() const throw();
    };
    
    /**
      sets the userfile to a nonstandard userfile.  
      @param Object* the object containing the userfile data.
      @return true if the file was parsed correctly and set, false otherwise
    */
    static bool set_userobj(Object*);
    
    
    /**
     returns true if name is a valid name.  name is valid if it is less than either 255 chars or FILENAME_MAX (whichever is shorter) and 
     contains letters, numbers, .s and _s.
     @param const string& username the username to test
     @return true if it's valid, false otherwise
    */
    static bool valid_name(const string& username);
  
    /**
	   loads a user if it exists, else throws an exception
	   
	   @param string user_name the user_name of the user to load
	   @return the User	   
	  */
	  
    User(const string& username) throw (UserException);    
    
		/**
	    gets the user's name
	    @return a pointer to a string of the user's name
	  */
    string name() const;
    
	  
		/**
		  gets all of this users' groups
		  @return a pointer to a vector of string*'s, each is a group this user is in. 
		*/
		const vector<const string*>* groups() const;
		
		/**
		  checks to see if user is in the group
		  @return true if user is in the group, false otherwise
		*/
    bool in_group(const string& group_name) const;
       
  };
}

#endif