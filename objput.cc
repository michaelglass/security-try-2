#include "utils.h"
#include <iostream>

#define USAGE_STRING  "Usage: objput -u username -g groupname objname\n" \
                      "  username        the username of the user putting the object.\n" \
                      "  groupname       the groupname of the user putting the object.\n" \
                      "  objname         the name the object (being piped in over stdin) will be called in the object store.\n"

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