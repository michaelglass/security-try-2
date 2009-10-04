#include <iostream>
#include <cctype>
#include "objectstore.h"
#define USAGE_STRING  "Usage: objtestacl -u username -g groupname -a access objname\n" \
                      "  username        the username for whom access is being tested.\n" \
                      "  groupname       the groupname of the username whose access is being tested.\n" \
                      "  access          the access (r,w,x,p or v) being tested\n" \
                      "  objname         the name of the object against which the user/group/access is being tested.\n"

int usage(const std::string& usage_string)
{
  std::cerr << usage_string;
  return 1;
}


int main(int argc, char* argv[])
{
  using namespace std;
  
  const string ALLOWED = ALLOWED_ACCESS_CHARS;
  
	int c;
  string username, groupname, objname;
  char access(0);
  
	
  objname = argv[argc-1];
	while((c = getopt(argc-1, argv, "u:g:a:")) != -1)
		switch(c)
		{
			case 'a':
				access = tolower(optarg[0]);
				break;
			case 'u':
				username = optarg;
				break;
			case 'g':
        groupname = optarg;
        break;
			default:
        return usage(USAGE_STRING);
		}
	if(username.length() == 0 || groupname.length() == 0 || access == 0)
    return usage(USAGE_STRING);
  if(ALLOWED.find(access) == string::npos)
    return usage("Error: invalid access char\n" USAGE_STRING);
  
    
  cout << "username: " << username << "\n"
      << "groupname: " << groupname << "\n"
      << "access char: " << access << "\n"
      << "object name: " << objname << "\n";
}