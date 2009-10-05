#include "utils.h"
#include "objectstore.h"
#include <stdio.h>

#define USAGE_STRING  "Usage: objsetacl -u username -g groupname objname\n" \
                      "  objput must also be piped a file (ie, objput < filename or cat file | objput)\n" \
                      "  username        the username of the user setting the ACL.\n" \
                      "  groupname       the groupname of the user setting the ACL.\n" \
                      "  objname         the name of the object whose ACL is being set.\n"

int main(int argc, char* argv[])
{    
  using namespace std;
  using namespace object_store;
  using namespace utils;

  string ownername, username, groupname, objname;
  if(! get_params(argc, argv, USAGE_STRING, username, groupname, ownername, objname) )
    return 1;
    
  if(isatty(fileno(stdin)) )
  {
    cerr << USAGE_STRING << endl << "error: objsetacl must be piped a file!" << endl;
    return 2;
  }
    

  try{
    auto_ptr<ACLObject> obj(new ACLObject(username, groupname, ownername, objname));
    if(obj->exists())
    {
      auto_ptr<Object> acl(obj->get_ACL());  
      cin >> *acl;
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
