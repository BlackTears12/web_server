#pragma once
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include "../common/user.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <sstream>

namespace beast = boost::beast;
using std::string;
using std::vector;
using boost::asio::ip::address;
using boost::asio::io_context;
using tcp = boost::asio::ip::tcp;
using namespace common;

namespace web_server
{
   struct Request
   {
      Request(User* usr,beast::flat_buffer &request) : args(){
         sender = usr;
         std::stringstream ss(beast::buffers_to_string(request));
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

   string hash(string key){
      //Key needs to be hashed with SHA256
      return key;
   }
}