#include "utils.h"
#include <iostream>

#define USAGE_STRING  "Usage: objgetacl -u username -g groupname objname\n" \
                      "  username        the username of the user getting the acl.\n" \
                      "  groupname       the groupname of the user getting the acl.\n" \
                      "  objname         the name of the object whose acl is being retrieved.\n"
                      
int main(int argc, char* argv[])
{    
  using namespace std;

  string username, groupname, objname;
  if(! utils::get_params(argc, argv, USAGE_STRING, username, groupname, objname) )
    return 1;

  cout << "username: " << username
      << "\ngroupname: " << groupname
      << "\nobjname: " << objname << "\n";
}