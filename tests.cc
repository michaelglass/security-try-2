#include "object.h"
#include <string>
#include <iostream>
#include <sstream>
#define LINE_LENGTH 100

void test(const std::string& test_name, bool pass )
{
  using namespace std;
  cout << "test: " << test_name;
  
  for(size_t i = test_name.length(); i < LINE_LENGTH; i++)
    cout << ".";
  
  cout << (pass ? "P" : "FAILED!!!") << "\n";
}

void object_tests()
{
  using namespace object_store;
  string valid_name = "dog@", invalid_name = "..", valid_path = "asdf/asdf/asdf", invalid_path = "asdf/asdf/asd";
  
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

    obj = new Object(valid_name);
    
    stringstream asdf("MONKEY NOISES!");
    asdf >> *obj;
    cout << *obj;
}


int main(int argc, char **argv)
{
  object_tests();
}

