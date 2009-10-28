#include "objectstore.h"
#include "utils.h"
#include <stdio.h>

#define USAGE_STRING  "Usage: objput objname\n" \
                      "  objname         the name the object (being piped in over stdin) will be called in the object store.\n"

int main(int argc, char* argv[])
{    
  using namespace std;
  using namespace object_store;
  utils::setup_setuid();
  
  string username, ownername, objname;
  vector<const string*>* groups = new vector<const string*>();
  
  if(! utils::get_params(argc, argv, USAGE_STRING, username, *groups, ownername, objname) )
    return 1;  
  
  try
  {
    auto_ptr<ACLObject> obj(new ACLObject(username, groups, ownername, objname));
    cin >> *obj;
  } 
  catch(exception& e)
  {
    cerr << USAGE_STRING << endl << e.what() << endl;
    return 3;
  }

}
