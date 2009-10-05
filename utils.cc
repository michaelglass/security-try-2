#include "utils.h"
#include <getopt.h>
#include <ctype.h>
#include <stdio.h>
#include <sstream> //int to string conversion
#include <iostream>

namespace utils
{
  using namespace std;
  
  int get_params(int argc, char *argv[], const string& usage_string, string& username, string& groupname, string& objname, string& ownername, bool get_last_arg)
  {
    int c;
    objname = argv[argc-1];
    while((c = getopt(argc-1, argv, "u:g:")) != -1)
    switch(c)
    {
      case 'u':
        username = optarg;
        break;
      case 'g':
        groupname = optarg;
        break;
      default:
        break;
    }
  
    if(!(username.length() && groupname.length() && objname.length()) ) //unless all of the params have a reasonable length
    {
      cerr << usage_string;
      return 0; //failure
    }
  
    size_t location;
  
    
    if(get_last_arg)
    {
      if((location = objname.find('+')) != string::npos)
      {
        ownername = objname.substr(0, location);
        objname = objname.substr(location + 1);
      }
      else
        ownername = username;
    }
    return 1; //success
 
  }

  int get_params(int argc, char *argv[], const string& usage_string, string& username, string& groupname, string& ownername, string& objname)
  {
    return get_params(argc, argv, usage_string, username, groupname, objname, ownername, true);
  }
  int get_params(int argc, char *argv[], const string& usage_string, string& username, string& groupname, string& objname)
  {
    string s;
    return get_params(argc, argv, usage_string, username, groupname, objname, s, false);
    
  }
  
  


  std::string itostr(int i)
  {
    stringstream s;
    s << i;
    return s.str();
  }
}
