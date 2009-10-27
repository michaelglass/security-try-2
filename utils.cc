#include "utils.h"
#include <getopt.h>
#include <ctype.h>
#include <stdio.h>
#include <sstream> //int to string conversion
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <grp.h>
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
        if(ownername.length() == 0 || objname.length() == 0)
        {
          cerr << usage_string << "\n can't have a '+' in the username or objname\n";
          return 0;
        }
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
  
  int get_userinfo(string& username, vector<string*>& groupnames)
  {
    int i = 0;
    
    char* login = getlogin();
    if(login) //if not null, ie if success 
      username = string(login);
    else
      return 1;
    
    gid_t *groups;
    int ngroups;
    long ngroups_max;
    
    ngroups_max = sysconf(_SC_NGROUPS_MAX) + 1;
    
    groups = (gid_t *)malloc(ngroups_max *sizeof(gid_t)) ;


    ngroups = getgroups(ngroups_max, groups);
    
    if(ngroups <= 0)
      return 1;

    for(int i = 0; i != ngroups; i++)
    {
      group *gr = getgrgid(groups[i]);
      groupnames.push_back(new string(gr->gr_name));
    }
        
    return 0;
  }
  
  /* Save the effective and real UIDs. */
  
  static uid_t euid, ruid;


  void setup_setuid(void)
  {
    ruid = getuid ();
    euid = geteuid ();
    undo_setuid ();
  }

  /* Restore the effective UID to its original value. */

  
  void
  do_setuid (void)
  {
    int status;

  #ifdef _POSIX_SAVED_IDS
    status = setuid (euid);
  #else
    status = setreuid (ruid, euid);
  #endif
    if (status < 0) {
      fprintf (stderr, "Couldn't set uid.\n");
      exit (status);
      }
  }


  /* Set the effective UID to the real UID. */

  void
  undo_setuid (void)
  {
    int status;

  #ifdef _POSIX_SAVED_IDS
    status = setuid (ruid);
  #else
    status = setreuid (euid, ruid);
  #endif
    if (status < 0) {
      fprintf (stderr, "Couldn't set uid.\n");
      exit (status);
      }
  }
  
  
}
