#include <iostream>
#include <string>

#define USAGE_STRING  "Usage: objlist -u username -g groupname [-l]\n" \
                      "  username        the username whose available objects are being listed.\n" \
                      "  groupname       the groupname of the username whose available objects are being listed.\n" \
                      "  -l              list all metadata associated with each object\n"

int usage(const std::string& usage_string)
{
  std::cerr << usage_string;
  return 1;
}

int main(int argc, char* argv[])
{
  using namespace std;
  
	int c;
  string username;
  string groupname;
	bool show_metadata_flag = false;
	
	while((c = getopt(argc, argv, "u:g:l")) != -1)
		switch(c)
		{
			case 'l':
				show_metadata_flag = true;
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
	if(username.length() == 0 || groupname.length() == 0)
    return usage(USAGE_STRING);
    
  cout << "username: " << username << "\n"
      << "groupname: " << groupname << "\n"
      << (show_metadata_flag ? "" : "not ")  << "showing metadata\n";
}