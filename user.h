#ifndef MGG2102_SECURITY_USER_H
#define MGG2102_SECURITY_USER_H

#ifndef OBJECTSTORE_PATH
#define OBJECTSTORE_PATH "objectstore"
#endif

#include <string>
#include <vector>
#include <memory>

namespace object_store
{
  using namespace std;

  class Object;
  
  class User
  {
    
    auto_ptr<string> _user_name;
    auto_ptr< vector<const string*> > _groups;
    
    static auto_ptr<Object> userobj;
  public:
        
    class UserException : public exception
    {
      bool _doesnt_exist;
      auto_ptr<string> _user_name;
      public:
       UserException(const string& user_name, bool doesnt_exist) throw();
       virtual ~UserException() throw();
       virtual const char* what() const throw();
    };
    
    /**
      gets a list of all the users
      @return vector of all the users
    */
    static const vector<const User*>* users();
    
    /**
      sets the userfile to a new userfile.  THIS NEEDS TO BE CALLED BEFORE FIRST-RUN
      
      syntax for userfiles is 
      username group group group
      there can be no trailing / leading newlines
      
      @param Object* the object containing the userfile data.
      @return true if the file was parsed correctly and set, false otherwise
    */
    static bool set_userobj(Object*);
    
    
    /**
     returns true if name is a valid name.  name is valid if it is less than either 255 chars or FILENAME_MAX (whichever is shorter) and 
     contains letters, numbers, .s and _s.
     @param const string& user_name the user_name to test
     @return true if it's valid, false otherwise
    */
    static bool valid_name(const string& user_name);
  
    /**
	   loads a user if it exists, else throws an exception
	   
	   @param string user_name the user_name of the user to load
	   @return the User	   
	  */
	  
    User(const string& user_name) throw (UserException);    
    
		User(const User& rhs);
    ~User();
    
		
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