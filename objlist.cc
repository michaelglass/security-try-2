#include "objectstore.h"
#include <iostream>
#include <string>
#include <sstream>

#define USAGE_STRING  "Usage: objlist -u username [-l]\n" \
                      "  username        the username whose available objects are being listed.\n" \
                      "  -l              list all metadata associated with each object\n"


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
  string username;
	bool show_metadata_flag = false;
	
	while((c = getopt(argc, argv, "u:l")) != -1)
		switch(c)
		{
			case 'l':
				show_metadata_flag = true;
				break;
			case 'u':
				username = optarg;
				break;
			default:
        return usage(USAGE_STRING);
		}
	if(username.length() == 0)
    return usage(USAGE_STRING);
    
  try{
    auto_ptr< vector<UserObject*> > objs(ACLObject::objects(username));
    vector<UserObject*>::iterator it;
    
    for(it = objs->begin(); it < objs->end(); it++)
    {
      UserObject* obj = *it;
      string name = obj->name();
      cout << name;
      
      if(show_metadata_flag)
      {
        //LINE LENGTH defaults to 50
        stringstream length;
        length << obj->length();
        for(int i = name.length() + length.str().length() + string("bytes").length(); i < 50; i++)
          cout << ".";
        cout << length.str() << " bytes";
      }
      cout << endl;
      delete obj;
    }
  }
  catch(exception& e)
  {
    cerr << USAGE_STRING << endl << e.what() << endl;
    return 2;
  }
    
}