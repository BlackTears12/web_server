#pragma once
#include "../common/user.hpp"
#include <vector>
#include <thread>
using std::vector;
using User = common::User;
namespace beast = boost::beast;

namespace web_server
{
   class User_Container{
   public:
   class NullUserException : std::exception{};
      User* get_user(string key);
      void new_user(string key);
   private:
      void find_user(string key,int startindex,int endindex,User* &usr);
      vector<User*> user_list;
   };

   User* User_Container::get_user(string key){
      User* usr = nullptr;
      find_user(key,0,user_list.size()-1,usr);
      if(!usr)
         throw NullUserException();
      return usr;
   }

   void User_Container::find_user(string key, int startindex,int endindex,User* &usr){
      if(usr)
         return;
      if(user_list[startindex+endindex/2]->get_UID() == key){
         usr = user_list[startindex+endindex/2];
         return;
      }
      std::thread(find_user,(key,startindex,(startindex+endindex/2)-1,usr)).detach();
      find_user(key,(startindex+endindex/2)+1,endindex,usr);
   }

}