#include "objectstore.h"
#include "utils.h"
#include <iostream>
#include <cctype>
#define USAGE_STRING  "Usage: objtestacl -a access objname\n" \
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
  utils::setup_setuid();
	int c;
  string ownername, username, objname;
  vector<const string*>* groups = new vector<const string*>();
  
  utils::get_userinfo(username, *groups);
  
  
  char access(0);
  
	
  objname = argv[argc-1];
  
	while((c = getopt(argc-1, argv, "a:")) != -1)
		switch(c)
		{
			case 'a':
				access = tolower(optarg[0]);
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
      auto_ptr<ACLObject> obj(new ACLObject(username, groups, ownername, objname));
      if(obj->exists())
      {
        switch(access)
        {
          case 'r':
            cout << (obj->can_read() ? "allowed" : "denied");
            break;
          case 'w':
            cout << (obj->can_write()? "allowed" : "denied");
            break;
          case 'x':
              cout << (obj->can_execute()? "allowed" : "denied");
            break;
          case 'p':
            cout << (obj->can_write_permissions()? "allowed" : "denied");
            break;
          case 'v':
            cout << (obj->can_view_permissions()? "allowed" : "denied");
            break;
          default: 
            return usage(USAGE_STRING);
        }
        cout << endl;
      }
      else
      {
        cout << "denied";
        // cerr << USAGE_STRING << endl << "object doesnt exist" << endl;
        return 2;
      }
    }
    catch(exception& e){
      cerr << USAGE_STRING << endl << e.what() << endl;
      return 3;
    }
}

