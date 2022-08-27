#pragma once
#include "../common/commonheader.hpp"
#include "../common/web_connection.hpp"
#include "../common/user.hpp"
#include <iostream>
#include <thread>
#include <sstream>

using namespace common;

namespace web_server
{
   struct Request
   {
      Request(User* usr,beast::flat_buffer request) : args(){
         sender = usr;
         std::stringstream ss(beast::buffers_to_string(request.data()));
         string str;
         ss >> str;
         if(str == "message")
            type = Request::MESSAGE;
         else throw UknownRequestType(str);
         while(ss >> str){
            args.push_back(str);
         }
      }
      class UknownRequestType : std::exception{
      public:
         UknownRequestType(string message){
            std::cerr << message;
         }
      };
      enum Type{MESSAGE};
      Type type;
      vector<string> args;
      User* sender;
   };
}