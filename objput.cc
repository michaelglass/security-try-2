#include "objectstore.h"
#include "utils.h"
#include <stdio.h>

#define USAGE_STRING  "Usage: objput -u username -g groupname objname\n" \
                      "  objput must also be piped a file (ie, objput < filename or cat file | objput)\n" \
                      "  username        the username of the user putting the object.\n" \
                      "  groupname       the groupname of the user putting the object.\n" \
                      "  objname         the name the object (being piped in over stdin) will be called in the object store.\n"

int main(int argc, char* argv[])
{    
  using namespace std;
  using namespace object_store;
  
  string username, groupname, ownername, objname;
  if(! utils::get_params(argc, argv, USAGE_STRING, username, groupname, ownername, objname) )
    return 1;
    
  // if(isatty(fileno(stdin)) )
  // {
  //   cerr << USAGE_STRING << endl << "error: objput must be piped a file!" << endl;
  //   return 2;
  // }
  
  try
  {
    auto_ptr<ACLObject> obj(new ACLObject(username, groupname, ownername, objname));
    cin >> *obj;
  } 
  catch(exception& e)
  {
    cerr << USAGE_STRING << endl << e.what() << endl;
    return 3;
  }

}
