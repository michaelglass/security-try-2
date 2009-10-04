#ifndef MGG2102_SECURITY_UTILS_H
#define MGG2102_SECURITY_UTILS_H

#include <string>

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
  

  /**
    returns a string representation of the int passed to it
    
    @param int i the int to convert to a string (ie, 1255)
    @return string representation (ie, "1255")
  */
  string itostr(int i);
  
}


#endif