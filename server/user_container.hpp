#pragma once
#include "../common/user.hpp"
#include "../common/web_connection.hpp"
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
      vector<User*> user_list;
   };
}