#include "commonheader.hpp"

namespace web_server
{
   class User{
   public:
      User(/* args */);
      ~User();

      string getUID(){return UID;}
   private:
      string UID;
      string passwHash;
   };
   
   User::User(/* args */)
   {
   }
   
   User::~User()
   {
   }
   
}