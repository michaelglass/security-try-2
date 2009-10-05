#include "acl.h"
#include <sstream>
#include <iostream>

namespace object_store
{
  ACL::ACLException::ACLException(const string& name) throw() : PermissionsObjectException(name, false)
  {}
  ACL::ACLException::~ACLException() throw()
  {}
  const char* ACL::ACLException::what() const throw()
  {
    return ("ACLException:  Can't write malformed ACL to object + \""+*_name+"\"").c_str();
  }
  
  
  ACL::ACL(const Object& obj, bool can_read, bool can_write) : PermissionsObject(obj, can_read, can_write){}
  ACL::ACL(const ACL& rhs) : PermissionsObject(rhs){}
  Object* ACL::clone() const{ return new ACL(*this); }

  istream& ACL::read(istream& is) throw(PermissionsObjectException) {
    
    // cerr << "\nreading new ACL";
    if(can_write())
    {
      stringstream new_acl;
      bool fail = false;
      bool pass = false;
      while(!fail && is.good())
      {
        string line;
        getline(is, line);
        // cerr << "\n" << line;
        if(line.length() > 0)
        {
          size_t curr_offset = 0;
          size_t next_offset = line.find('.', curr_offset);
          if(next_offset == string::npos)
          {
            fail = true;
            break;
          }
          string user = line.substr(curr_offset, next_offset-curr_offset);
          if(user == "*" || User::valid_name(user)) //username match!
          {
            curr_offset = next_offset + 1; //skipping *
            next_offset = line.find(' ', curr_offset); 
            if(next_offset == string::npos)
            {
              fail = true;
              break;
            }
            string group = line.substr(curr_offset, next_offset - curr_offset);
            if(group == "*" || User::valid_name(group) )
            {
              string permissions = line.substr(next_offset+1);
              string::iterator it;
              for(it = permissions.begin(); it < permissions.end(); it++)
                switch(*it)
                {
                  case 'r':
                  case 'w':
                  case 'x':
                  case 'p':
                  case 'v':
                    //we're fine
                    break;
                  default:
                    fail = true;
                    break;
                }
            }
          }
          pass = true;
          new_acl << line << endl;
        }
        else
          fail = true;
      
      }
      if(fail || !pass)
      {
        // cerr << "\nfailed parsing, throwing error";
        ACLException ae(*_name);
        throw ae;
      }
      else
      {
        // cerr << "\ndidnt fail, writing acl";
        return PermissionsObject::read(new_acl);
      }
    }
    // cerr << "\n can't write, returning PermissionsObject version (should just be is eventually)";
    return PermissionsObject::read(is);
  }

}
