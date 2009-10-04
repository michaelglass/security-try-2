#include "utils.h"
#include "objectstore.h"

#define USAGE_STRING  "Usage: objgetacl -u username -g groupname objname\n" \
                      "  username        the username of the user getting the acl.\n" \
                      "  groupname       the groupname of the user getting the acl.\n" \
                      "  objname         the name of the object whose acl is being retrieved.\n"
                      
int main(int argc, char* argv[])
{    
  using namespace std;
  using namespace object_store;
  using namespace utils;

  string ownername, username, groupname, objname;
  if(! get_params(argc, argv, USAGE_STRING, username, groupname, ownername, objname) )
    return 1;
    
  try{
    auto_ptr<ACLObject> obj(new ACLObject(username, groupname, ownername, objname));
    if(obj->exists())
    {
      auto_ptr<Object> acl(obj->get_ACL());  
      cout << *acl;
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