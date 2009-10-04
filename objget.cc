
#include "utils.h"
#include "objectstore.h"

#include <string>
#include <GetOpt.h>
#include <ctype.h>
#include <stdio.h>
#include <sstream> //int to string conversion
#include <iostream>
#include <memory>

#define USAGE_STRING  "Usage: objget -u username -g groupname objname\n" \
                      "  username        the username of the user getting the object.\n" \
                      "  groupname       the groupname of the user getting the object.\n" \
                      "  objname         the name of the object being retrieved.\n"

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
      cout << *obj;
    else
      cerr << USAGE_STRING << endl << "object doesnt exist" << endl;
  }
  catch(exception& e){
    cerr << USAGE_STRING << endl << e.what() << endl;
  }
}
