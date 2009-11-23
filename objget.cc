#include "utils.h"
#include "objectstore.h"

#define USAGE_STRING  "Usage: objget [-k passphrase] objname\n" \
                      "  objname          the name of the object being retrieved.\n" \
                      "  [-k passphrase]  optional -k and passphrase with which to encrypt data.\n"


int main(int argc, char* argv[])
{    
  using namespace std;
  using namespace object_store;
  using namespace utils;
  utils::setup_setuid();

  string ownername, username, objname, key;
  vector<const string*>* groups = new vector<const string*>();
  unsigned char* hash = (unsigned char*) malloc(16);
  unsigned int md5_len;
  bool use_key;
    
  if(! get_params(argc, argv, USAGE_STRING, username, *groups, ownername, objname, key) )
    return 1;
  
  use_key = (key != "");
  if(use_key)
  {
    md5(&key, hash, &md5_len);
    if(md5_len != 16)
    {
      cerr << "can't hash passphrase, try another?" << endl << USAGE_STRING;
      return 1;
    }
  }  
  try{
    auto_ptr<ACLObject> obj(new ACLObject(username, groups, ownername, objname));
    if(obj->exists())
    {
      if(use_key)
      {
        if(read_encrypted((*obj), cout, hash) != 0)
          cerr << "can't decrypt file.  passphrase is probably wrong" << endl << USAGE_STRING;
      }
      else
        cout << *obj;
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
