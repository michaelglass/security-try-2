#include "objectstore.h"
#include <iostream>
#include <cctype>
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
  using namespace object_store;
	int c;
  string username, groupname, objname, ownername;
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
  
  
  size_t location;
  
  if((location = objname.find('+')) != string::npos)
  {
    ownername = objname.substr(0, location);
    objname = objname.substr(location + 1);
  }
  else
    ownername = username;
  
  try{
      auto_ptr<ACLObject> obj(new ACLObject(username, groupname, ownername, objname));
      if(obj->exists())
      {
        switch(access)
        {
          case 'r':
            cout << (obj->can_read() ? "CAN" : "CAN'T") << " READ";
            break;
          case 'w':
            cout << (obj->can_write()? "CAN" : "CAN'T") << " WRITE";
            break;
          case 'x':
              cout << (obj->can_execute()? "CAN" : "CAN'T")<< " EXECUTE";
            break;
          case 'p':
            cout << (obj->can_write_permissions()? "CAN" : "CAN'T")<< " CHANGE PERMISSIONS";
            break;
          case 'v':
            cout << (obj->can_view_permissions()? "CAN" : "CAN'T")<< " VIEW PERMISSIONS";
            break;
          default: 
            return usage(USAGE_STRING);
        }
        cout << endl;
      }
      else
      {
        cerr << USAGE_STRING << endl << "object doesnt exist" << endl;
        return 2;
      }
    }
    catch(exception& e){
      cerr << USAGE_STRING << endl << e.what() << endl;
      return 3;
    }
}

