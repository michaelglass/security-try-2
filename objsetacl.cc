#include "utils.h"
#include <iostream>

#define USAGE_STRING  "Usage: objsetacl -u username -g groupname objname\n" \
                      "  username        the username of the user setting the ACL.\n" \
                      "  groupname       the groupname of the user setting the ACL.\n" \
                      "  objname         the name of the object whose ACL is being set.\n"

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