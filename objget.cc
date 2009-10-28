#include "utils.h"
#include "objectstore.h"

#define USAGE_STRING  "Usage: objget objname\n" \
                      "  objname         the name of the object being retrieved.\n"

int main(int argc, char* argv[])
{    
  using namespace std;
  using namespace object_store;
  using namespace utils;

  string ownername, username, objname;
  vector<const string*>* groups = new vector<const string*>();
    
  if(! get_params(argc, argv, USAGE_STRING, username, *groups, ownername, objname) )
    return 1;
    
  try{
    auto_ptr<ACLObject> obj(new ACLObject(username, groups, ownername, objname));
    if(obj->exists())
      cout << *obj;
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
