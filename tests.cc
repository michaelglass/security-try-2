#include "object.h"
#include "user.h"
#include <string>
#include <iostream>
#include <sstream>
#define LINE_LENGTH 150

void title(const std::string& title)
{
  using namespace std;
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
  using namespace std;
  cout << "test: " << test_name;
  size_t line_length = LINE_LENGTH - 15;
  for(size_t i = test_name.length(); i < line_length; i++)
    cout << ".";
  
  cout << (pass ? "PASS!!!!!" : "!!!FAILED") << "\n";
}

void object_tests()
{
  using namespace object_store;
  title("TESTING OBJECTS");
  
  string valid_name = "dog@", invalid_name = "..", valid_path = "test_dir/asdf/asdf/asdf/", invalid_path = "asdf/asdf/asd";
  
  //path testing
  test(valid_name + " is a valid name", !Object::valid_name(valid_name));
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
  using namespace object_store;
  title("TESTING USERS");
  
  Object* new_user_obj = new Object("user_file","test_dir");
  stringstream test_userfile("michael group1 group2\nelena group2 group3");
  test_userfile >> *new_user_obj;
  test("test userfile should validate and be set", User::set_userobj(new_user_obj) );
  test_userfile << "\n\n!@#"; //invalid now
  test_userfile >> *new_user_obj;
  test("test userfile should NOT validate and be set", !User::set_userobj(new_user_obj) );
  string username = "elena";
  string group = "group2";
  test("user \""+username+"\" should be valid", User::valid_name(username));
  User* usr;
  test("creating a user with username\""+username+"\" should not throw an exception", usr = new User(username) );
  test("that user should be in the group \""+group+"\".", usr->in_group(group) );
  group += "arg";
  test("that user should NOT be in the group \""+group+"\".", !usr->in_group(group) );
  delete usr;

  bool exception_thrown = false;
  username = "asdf!!@#$";
  try{
    usr = new User(username);
  }
  catch(const User::UserException &e)
  {
    exception_thrown = true;
  }
  
  test("an invalid name (\""+username+"\") causes an exception to be thrown", exception_thrown);
  exception_thrown = false;
  username = "bob";
  try{
    usr = new User(username);
  }
  catch(const User::UserException &e)
  {
    exception_thrown = true;
  }
  test("an unknown user (\""+username+"\") causes an exception to be thrown", exception_thrown);  
}


int main(int argc, char **argv)
{
  object_tests();
  user_tests();
}

