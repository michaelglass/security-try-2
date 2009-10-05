#include "objectstore.h"
#include "utils.h"
#include <stdio.h>

#define USAGE_STRING  "Usage: objsetuserobj -u username -g groupname objname\n" \
                      "  takes a users file and copies it into the userfile\n"\
                      "  userfile is in the following format: \n" \
                      "user1 group1 group2\n" \
                      "user2 gruop2 group3\n" \
                      "it may have no extraneous newlines\n"\
                      "any user who is given no groups effectively has no access beyond objlist\n" \
                      "*note setting userobj does not modify ex-users data.\n"

int main(int argc, char* argv[])
{    
  using namespace std;
  using namespace object_store;

  string username, groupname, ownername, objname;
  if(! utils::get_params(argc, argv, USAGE_STRING, username, groupname, ownername, objname) )
    return 1;

  try
  {
    auto_ptr<ACLObject> obj(new ACLObject(username, groupname, ownername, objname));
    if(!User::set_userobj(obj.get()))
    {
      cerr << USAGE_STRING << endl << "error: malformed user file" << endl;
      return 2;
    }
    else
    {
      UserObject::make_homedirs();
    }
  } 
  catch(exception& e)
  {
    cerr << USAGE_STRING << endl << e.what() << endl;
    return 3;
  }

}
