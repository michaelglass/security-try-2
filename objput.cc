#include "objectstore.h"
#include "utils.h"
#include <stdio.h>

#define USAGE_STRING  "Usage: objput [-k passphrase] objname\n" \
                      "  objname          the name the object (being piped in over stdin) will be called in the object store.\n" \
                      "  [-k passphrase]  optional -k and passphrase with which to encrypt data.\n"

int main(int argc, char* argv[])
{    
  using namespace std;
  using namespace object_store;
  using namespace utils;
  setup_setuid();
  
  string username, ownername, objname, key;
  vector<const string*>* groups = new vector<const string*>();
  unsigned char hash[16];
  unsigned int md5_len;
  bool use_key;

  if(! utils::get_params(argc, argv, USAGE_STRING, username, *groups, ownername, objname, key) )
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
  try
  {
    auto_ptr<ACLObject> obj(new ACLObject(username, groups, ownername, objname));
    if(use_key)
    {
      if( write_encrypted(cin, *obj, hash) !=0 )
        cerr <<"can't write encrypted.  uknown error." << endl << USAGE_STRING;
    }
    else
      cin >> *obj;
    
  } 
  catch(exception& e)
  {
    cerr << USAGE_STRING << endl << e.what() << endl;
    return 3;
  }

}
