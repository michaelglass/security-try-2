#ifndef MGG2102_SECURITY_UTILS_H
#define MGG2102_SECURITY_UTILS_H

#include <string>
#include <vector>
#include <iostream>
#include "object.h"

namespace utils
{
  using namespace std;
  
  /**
    gets the username, group, and objname from the command line.  prints the usage string if it fails at grabbing all three.
  
    @param int argc the program's argument count
    @param char* argv[] the program's argument array
    @param const string* usage_stinrg the usage string to print if the parsing fails
    @param string& username the username string to populate
    @param string& group the group string to populate
    @param string& objname the objname to populate
  
    @return returns 0 on failure, 1 on success
  */
  int get_params(int argc, char **, const string& usage_string, string& username, string& groupname, string& ownername, string& objname);

  // same thing, doesnt split objname
  int get_params(int argc, char **, const string& usage_string, string& username, string& groupname, string& objname);
  //vector of groups instead of one groupname
  int get_params(int argc, char* argv[], const string& usage_string, string& username, vector<const string*>& groupnames, string& ownername, string& objname);
  //and a keyphrase
  int get_params(int argc, char* argv[], const string& usage_string, string& username, vector<const string*>& groupnames, string& ownername, string& objname, string& key);
  
  int get_obj_and_owner(int argc, char*argv[], const string& usage_string, string& ownername, string& objname, string& username);

  /**
    returns a string representation of the int passed to it
    
    @param int i the int to convert to a string (ie, 1255)
    @return string representation (ie, "1255")
  */
  string itostr(int i);
  
  
  /**
    populates user info
    
    @param string& username
    @param string& groupname
    
    @return 1 on success.  0 on failure  
  */
  int get_userinfo(string& username, vector<const string*>& groupname);

/*sets up setuid vars  */
  void setup_setuid(void);
 
 /** the following two code snippets are taken from here:
 http://www.aquaphoenix.com/ref/gnu_c_library/libc_438.html
 */
/* Restore the effective UID to its original value. */

  void do_setuid (void);

/* Set the effective UID to the real UID. */

  void undo_setuid (void);
  
  /** 
  hashes the input string.  unpredictable behavior with input > 255 chars.
  @param string* input the input string
  @param unsigned char* output the output char*
  @param unsigned int* length the output int*'s length
  */
  void md5(string* input, unsigned char* output, unsigned int* length);
  
  int write_encrypted(istream& from, object_store::Object& to, unsigned char* key);
  int read_encrypted(object_store::Object& from, ostream& to, unsigned char* key);
}


#endif
