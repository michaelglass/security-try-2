#include "objectstore.h"
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <dirent.h>
#include <errno.h>
#include <string>

#define LINE_LENGTH 150

using namespace std;
using namespace object_store;

void title(const std::string& title)
{
  cout << endl << endl;
  size_t length = title.length();
  size_t i;
  
  for(i = 0; i < LINE_LENGTH; i++)
    cout << "=";
  
  cout << endl;
  
  size_t before_text = (LINE_LENGTH - length)/2;
  for(i = 0; i < before_text; i++)
    cout << " ";
  cout << title << endl;
  for(i = 0; i < LINE_LENGTH; i++)
    cout << "=";
  cout << endl;
}

void test(const std::string& test_name, bool pass )
{
  cout << "test: " << test_name;
  size_t line_length = LINE_LENGTH - 15;
  for(size_t i = test_name.length(); i < line_length; i++)
    cout << ".";
  
  cout << (pass ? "PASS!!!!!" : "!!!FAILED") << "\n";
}

void object_tests()
{
  title("TESTING OBJECTS");
  
  string valid_name = "dog]", invalid_name = "..", valid_path = "test_dir/asdf/asdf/asdf/", invalid_path = "asdf/asdf/asd";
  
  //path testing
  test(valid_name + " is a valid name", Object::valid_name(valid_name));
  test(invalid_name + " is not a valid name", !Object::valid_name(invalid_name));
  test(valid_path + " is a valid path", Object::valid_path(valid_path));
  test("a blank string is a valid path", Object::valid_path(""));
  test(invalid_path + " is an invalid path", !Object::valid_path(invalid_path));
  
  Object* obj;
  test("a valid name and no path will create an Object", obj = new Object(valid_name));

  delete obj;

  test("a valid name and path will create an Object", obj = new Object(valid_name, valid_path));
  delete obj;
  
  bool exeption_thrown = false;
   try
  {
    new Object(invalid_name);
  }
  catch (const Object::ObjectException &e)
  {
    exeption_thrown = true;
  }
  test("an invalid name causes an error to be thrown", exeption_thrown);

  obj = new Object(valid_name, valid_path);
  
  string the_right_result = "test string";
  stringstream my_in(the_right_result), my_out;
  
  my_in >> *obj;
  my_out << *obj;
  test("when I input and output file contents to my object the results are the same", my_in.str() == my_out.str());
  
  //now that I've created my obj, it should exist.
  
  test("now object should exist that we've written to it", obj->exists());
  delete obj;
}

void user_tests()
{
  title("TESTING USERS");
  
  Object* new_user_obj = new Object("user_file","test_dir");
  stringstream test_userfile("michael group1 group2\nelena group2 group3");
  test_userfile >> *new_user_obj;
  test("test userfile should validate and be set", User::set_userobj(new_user_obj) );
  test_userfile << "\n\n!@#"; //invalid now
  test_userfile >> *new_user_obj;
  test("test userfile should NOT validate and be set", !User::set_userobj(new_user_obj) );
  delete new_user_obj;
  string user_name = "elena";
  string group = "group2";
  test("user \""+user_name+"\" should be valid", User::valid_name(user_name));
  User* usr;
  test("creating a user with user_name\""+user_name+"\" should not throw an exception", usr = new User(user_name) );
  test("that user should be in the group \""+group+"\".", usr->in_group(group) );
  group += "arg";
  test("that user should NOT be in the group \""+group+"\".", !usr->in_group(group) );
  delete usr;

  bool exception_thrown = false;
  user_name = "asdf!!@#$";
  try{
    usr = new User(user_name);
  }
  catch(const User::UserException &e)
  {
    exception_thrown = true;
  }
  
  test("an invalid name (\""+user_name+"\") causes an exception to be thrown", exception_thrown);
  exception_thrown = false;
  user_name = "bob";
  try{
    usr = new User(user_name);
  }
  catch(const User::UserException &e)
  {
    exception_thrown = true;
  }
  test("an unknown user (\""+user_name+"\") causes an exception to be thrown", exception_thrown);  
}

void userobject_tests()
{
  
  title("TESTING USEROBJECTS");
  //initialization
  UserObject::make_homedirs();
  
  UserObject* obj;
  test("creating a userobject with a valid name and user doesn't throw an exception", obj = new UserObject("elena", "isafile"));
  delete obj;
  bool exception_thrown = false;
  try
  {
    obj = new UserObject("elena", "isafile]");
  } catch( UserObject::UserObjectException &e )
  { 
    exception_thrown = true;
  }
  test("creating a userobject with an invalid name and user throws an exception", exception_thrown);
  exception_thrown = false;
  try
  {
    obj = new UserObject("elenar", "isafile]");
  } catch( User::UserException &e )
  { 
    exception_thrown = true;
  } catch (Object::ObjectException &e)
  {
    exception_thrown = true;
  }
  test("creating a userobject with an invalid username throws an exception", exception_thrown);


  
  obj = new UserObject("elena", "isafile");
  
  string the_right_result = "test string";
  stringstream my_in(the_right_result), my_out;
  
  my_in >> *obj;
  my_out << *obj;
  test("when I input and output file contents to my object the results are the same", my_in.str() == my_out.str());  
  delete obj;
}

void permissionsobject_tests()
{
  title("TESTING PERMISSIONSOBJECTS");
  Object *obj, *permObj;
  obj = new UserObject("elena", "isafile");
  
  
  string init_str = "initial object contents";
  stringstream init_in(init_str), init_out;
  init_in >> *obj;
  
  permObj = new PermissionsObject(*obj, false, true);
  
  bool exception_thrown = false;
  try
  {
    init_out << *permObj;
  } catch (PermissionsObject::PermissionsObjectException& e)
  {
    exception_thrown = true;
  }
  
  test("unreadable/writable permissions object should not be able to read a file", init_out.str() == "");
  test("unreadable/writable permissions object when it tries an exception should be thrown", exception_thrown);
  
  string new_contents = "new contents";
  stringstream new_in(new_contents);
  
  new_in >> *permObj;
  init_out << *obj;
  
  test("writing to it should actually write though", init_out.str() == new_contents);
  delete permObj;
  
  permObj = new PermissionsObject(*obj, true, false);
  
  init_out.str(""); //reset
  init_out << *permObj;
  test("readable/unwritable permissions object should be able to read the file", init_out.str() == new_contents);
  exception_thrown = false;
  try{
    init_in >> *permObj;
  } catch(PermissionsObject::PermissionsObjectException& e)
  {
    exception_thrown = true;
  }
  
  test("but writing to it should leave the data unchanged", init_out.str() != init_str);
  test("and an exception should be thrown", exception_thrown);
  delete obj;
  delete permObj;
}

void ACL_tests()
{
  title("TESTING ACLS");
  Object *acl;
  acl = new ACL(UserObject("elena", "anACL"));
  stringstream my_in, my_out;
  my_in << "elena.* rwpx\n*.* prwx";
  bool exception_thrown = false;
  try
  {
    my_in >> *acl;
  } catch (...)
  {
    exception_thrown = true;
  }
  test("I can write a valid ACL to the ACL without it throwing an error", !exception_thrown);
  
  my_in.clear();
  my_in.str("*.* ASDF");
  exception_thrown = false;
  try
  {
    my_in >> *acl;
  } catch (...)
  {
    exception_thrown = true;
  }
  test("but if I write an invalid ACL, i get an error", exception_thrown);
  delete acl;
}

void ACLObject_tests()
{
  title("TESTING ACLObjects");
  
  Object *aclobj;
  
  aclobj = new ACLObject("elena", "group3", "elena", "newfile");
  stringstream str;
  str << "first line....";
  str >> *aclobj;
  Object* acl = ((ACLObject*)aclobj)->get_ACL();

  str.clear();
  str.str("elena.* rwxpv\nmichael.* rwx");
  str >> *acl;
  delete acl;
  delete aclobj;
  
  aclobj = new ACLObject("michael", "group1", "elena", "newfile");
  ACLObject* testing;
  testing = (ACLObject*) aclobj;
  
  test("so now that I've set the permissions, michael can read:", testing->can_read());
  test("write", testing->can_write());
  test("ex", testing->can_execute());
  test("not view", !testing->can_view_permissions());
  test("not change p", !testing->can_write_permissions());

  PermissionsObject* p = testing->get_ACL();

  test("so the permissions are right but can I actually access the acl?", (!p->can_read()) && (!p->can_write()));
  
  
  delete aclobj;
}

int main(int argc, char **argv)
{
  object_tests();
  user_tests();
  userobject_tests();
  permissionsobject_tests();
  ACL_tests();
  ACLObject_tests();
}

