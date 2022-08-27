#include "user_container.hpp"
namespace web_server
{
   User* User_Container::get_user(string key){
      for(User* &e : user_list){
         if(e->get_UID() == key)
            return e;
      }
      throw NullUserException();
   }
}