#include <openssl/evp.h>
#include "utils.h"
#include <getopt.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream> //int to string conversion
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>
namespace utils
{
  using namespace std;
  
  int get_params(int argc, char *argv[], const string& usage_string, string& username, string& groupname, string& objname, string& ownername, bool get_last_arg)
  {
    int c;
    objname = argv[argc-1];
    while((c = getopt(argc-1, argv, "u:g:")) != -1)
    switch(c)
    {
      case 'u':
        username = optarg;
        break;
      case 'g':
        groupname = optarg;
        break;
      default:
        break;
    }
  
    if(!(username.length() && groupname.length() && objname.length()) ) //unless all of the params have a reasonable length
    {
      cerr << usage_string;
      return 0; //failure
    }
  
    size_t location;
  
    
    if(get_last_arg)
    {
      if((location = objname.find('+')) != string::npos)
      {
        ownername = objname.substr(0, location);
        objname = objname.substr(location + 1);
        if(ownername.length() == 0 || objname.length() == 0)
        {
          cerr << usage_string << "\n can't have a '+' in the username or objname\n";
          return 0;
        }
      }
      else
        ownername = username;
    }
    return 1; //success
 
  }

  int get_params(int argc, char *argv[], const string& usage_string, string& username, string& groupname, string& ownername, string& objname)
  {
    return get_params(argc, argv, usage_string, username, groupname, objname, ownername, true);
  }
  int get_params(int argc, char *argv[], const string& usage_string, string& username, string& groupname, string& objname)
  {
    string s;
    return get_params(argc, argv, usage_string, username, groupname, objname, s, false);
    
  }
  
  int get_params(int argc, char* argv[], const string& usage_string, string& username, vector<const string*>& groupnames, string& ownername, string& objname, string& key)
  {
    if(argc != 2 && argc != 4)
    {
      cerr << usage_string;
      return 0;
    }
    if(argc == 2)
    {
      key = "";
    }
    else
    {
      char key_char = argv[1][0];
      if(key_char == 'k' || 'K')
        key = string(argv[2]);
      else
      {
        cerr << usage_string;
        return 0;
      } 
    }
    
    if(get_userinfo(username, groupnames))
      return get_obj_and_owner(argc, argv, usage_string, ownername, objname, username);
    else
      return 0;
  }
  
  int get_params(int argc, char* argv[], const string& usage_string, string& username, vector<const string*>& groupnames, string& ownername, string& objname)
  {
    string key;
    int rval = get_params(argc, argv, usage_string, username, groupnames, ownername, objname, key);
    if(key == "" && rval)
      return rval;
    else
    {
      cerr << usage_string;
      return 0;
    }
      
  }
    
  int get_obj_and_owner(int argc, char*argv[], const string& usage_string, string& ownername, string& objname, string& username)
  {
    size_t location;
    objname = argv[argc-1];
    if(objname.length() == 0)
    {
      cerr << usage_string;
      return 0;
    }
    if((location = objname.find('+')) != string::npos)
    {
      ownername = objname.substr(0, location);
      objname = objname.substr(location + 1);
      if(ownername.length() == 0 || objname.length() == 0)
      {
        cerr << usage_string << "\n can't have a '+' in the username or objname\n";
        return 0;
      }
    }
    else
      ownername = username;
    return 1;
  }
  
  std::string itostr(int i)
  {
    stringstream s;
    s << i;
    return s.str();
  }
  
  int get_userinfo(string& username, vector<const string*>& groupnames)
  {
    struct passwd *pwd;
    pwd = getpwuid(getuid());
    if(pwd->pw_name)
      username = pwd->pw_name;
    else
      return 0;
    
    int ngroups;
    long ngroups_max = sysconf(_SC_NGROUPS_MAX) + 1;
    gid_t groups[ngroups_max];
    

    ngroups = getgroups(ngroups_max, groups);
    
    if(ngroups <= 0)
      return 0;

    for(int i = 0; i != ngroups; i++)
    {
      group *gr = getgrgid(groups[i]);
      groupnames.push_back(new string(gr->gr_name));
    }
        
    return 1;
  }
  
  /* Save the effective and real UIDs. */
  
  static uid_t euid, ruid;
  static bool is_setuid = false;

  void setup_setuid(void)
  {
    is_setuid = true;
    ruid = getuid ();
    euid = geteuid ();
    undo_setuid ();
  }

  /* Restore the effective UID to its original value. */

  
  void
  do_setuid (void)
  {
    if(is_setuid)
    {
      int status;

    #ifdef _POSIX_SAVED_IDS
      status = setuid (euid);
    #else
      status = setreuid (ruid, euid);
    #endif
      if (status < 0) {
        fprintf (stderr, "Couldn't set uid.\n");
        exit (status);
        }
    }
  }


  /* Set the effective UID to the real UID. */

  void
  undo_setuid (void)
  {
    if(is_setuid)
    {
      int status;

    #ifdef _POSIX_SAVED_IDS
      status = setuid (ruid);
    #else
      status = setreuid (euid, ruid);
    #endif
      if (status < 0) {
        fprintf (stderr, "Couldn't set uid.\n");
        exit (status);
        }
    }
  }
  void md5(string* input, unsigned char* output, unsigned int* length)
  {
    EVP_MD_CTX md5_me;
    EVP_DigestInit(&md5_me, EVP_md5());
    EVP_DigestUpdate(&md5_me, input->c_str(), input->length());
    EVP_DigestFinal(&md5_me, output, length);
  }
  
  int write_encrypted(istream& from, object_store::Object& to, unsigned char* key)
  {
    const unsigned int IV_LENGTH = 16, INBUF_LEN = 1024, OUTBUF_LEN = 1024;
    bool fail = false;
    unsigned char iv[IV_LENGTH];
    const EVP_CIPHER *cipher = EVP_aes_128_cbc();
    EVP_CIPHER_CTX ctx;
    char inbuf[INBUF_LEN], outbuf[OUTBUF_LEN];
    int inlen, outlen;
    ifstream urandom;
    fstream crypted;
    
    //gen random iv
    urandom.open("/dev/urandom");
    if (urandom.good()) {
      urandom.read((char*)iv, IV_LENGTH * sizeof(unsigned char));
      int read = urandom.gcount();
      urandom.close();
      if(read != IV_LENGTH)
        return 2;
    } 
    else
      return 1;

    //now lets get the cipher ready...
    EVP_CIPHER_CTX_init(&ctx);
    EVP_EncryptInit_ex(&ctx, cipher, NULL, key, iv);    

    //now lets write the encrypted version to a temp file...

    umask(S_IRWXG | S_IRWXO); //prevent others from reading tmp file
    char *tmpname = strdup("/tmp/tmpfileXXXXXX");
    mkstemp(tmpname);
    crypted.open(tmpname);
    //first 16 bytes are the iv
    crypted.write((char*)iv, sizeof(unsigned char) * IV_LENGTH);
    if(crypted.bad())
      fail = true;

    while(from.good() && !fail)
    {
      from.read(inbuf, INBUF_LEN);
      inlen = from.gcount();
      if(!EVP_EncryptUpdate(&ctx, (unsigned char*) outbuf, &outlen, (unsigned char*) inbuf, inlen))
      {
        fail = true;
        break;
      }
      crypted.write(outbuf, sizeof(unsigned char) * outlen);
      if(crypted.bad()) //then write encrypted chunk...
        return 5;
    }
    
    if(fail || !EVP_EncryptFinal_ex(&ctx,(unsigned char*) outbuf, &outlen))
      fail = true;
    else
      crypted.write(outbuf, sizeof(unsigned char) * outlen); //then write encrypted chunk...
    if(crypted.bad())
      fail = true;
    
    EVP_CIPHER_CTX_cleanup(&ctx);
    if(!fail)
    {
      crypted.seekg(ios::beg);
      crypted >> to;
    }
    crypted.close();
    remove(tmpname);
    if(fail)
      return 1;
    else
      return 0;
  }
  
  int read_encrypted(object_store::Object& from, ostream& to, unsigned char* key)
  {
    bool fail = false;
    const unsigned int IV_LENGTH = 16, INBUF_LEN = 1024, OUTBUF_LEN = 1024;
    unsigned char iv[IV_LENGTH];
    const EVP_CIPHER *cipher = EVP_aes_128_cbc();
    EVP_CIPHER_CTX ctx;
    char inbuf[INBUF_LEN], outbuf[OUTBUF_LEN];
    int inlen, outlen;
    ifstream urandom;
    fstream crypted, decrypted;
    
    //write object to temp file...

    umask(S_IRWXG | S_IRWXO); //prevent others from reading tmp file
    char *tmpname = strdup("/tmp/tmpfileXXXXXX");
    mkstemp(tmpname);
    crypted.open(tmpname);
    
    char *tmpname2 = strdup("/tmp/tmpfileXXXXXX");
    mkstemp(tmpname2);
    decrypted.open(tmpname2);
    
    crypted << from;
    crypted.seekg(0, ios::beg);
    
    //read iv out of crypted
    crypted.read((char*)iv, IV_LENGTH*sizeof(unsigned char));
    if(crypted.gcount() != IV_LENGTH)
      fail = true;
    
    //now lets get the cipher ready...
    EVP_CIPHER_CTX_init(&ctx);
    if(!fail)
      EVP_DecryptInit_ex(&ctx, cipher, NULL, key, iv);    

    //now lets read the encrypted version to a decrypted file...
        
    while(!fail && crypted.good())
    {
      crypted.read(inbuf, INBUF_LEN);
      inlen = crypted.gcount();
      if(!EVP_DecryptUpdate(&ctx, (unsigned char*) outbuf, &outlen, (unsigned char*) inbuf, inlen))
      {
        fail = true;
        break;
      }
      if(!fail)
        decrypted.write(outbuf, sizeof(unsigned char) * outlen);
      if(decrypted.bad()) //then write encrypted chunk...
        fail = true;
    }
    if(fail || !EVP_DecryptFinal_ex(&ctx,(unsigned char*) outbuf, &outlen))
      fail = true;
    else
      decrypted.write(outbuf, sizeof(unsigned char) * outlen); //then write encrypted chunk...
    if(decrypted.bad())
      fail = true;
    
    EVP_CIPHER_CTX_cleanup(&ctx);
    if(!fail)
    {
      decrypted.seekp(0, ios::beg);
      to << decrypted.rdbuf();
    }
    decrypted.close();
    remove(tmpname2);
    crypted.close();
    remove(tmpname);
    if(fail)
      return 1;
    else
      return 0;
  }  
}
