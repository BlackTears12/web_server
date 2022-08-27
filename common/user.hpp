#pragma once
#include "commonheader.hpp"

namespace common
{
   class User{
   public:
      User(/* args */){}
      ~User(){}

      string get_UID(){return UID;}
      string get_passwHash(){return passwHash;}
      string get_privatekey(){return privkey;}
   private:
      string UID;
      string passwHash;
      string privkey;
   };
}