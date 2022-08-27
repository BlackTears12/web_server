#pragma once
#include "user_container.hpp"
#include "requests.hpp"
#include "../common/hash.hpp"
using namespace common;

namespace web_server
{
   struct login_data{
      string username = "";
      string passwHash = "";
   };

   class Web_Server{
   public:
      static Web_Server* Instance();
      static void Destroy();
      void process_socket_connection(tcp::socket q);
   private:
      static Web_Server* instance;
      User_Container* user_containter;
   
      Web_Server();
      ~Web_Server();
      void process_request(User* sender,beast::flat_buffer &request);
      void send_message(User* sender,vector<string> args);
      void parse_login_buffer(beast::flat_buffer &buffer,login_data &data);
      User* login(websocket::stream<tcp::socket> &ws);

   class FailedLoginException : public std::exception{
   public:
      FailedLoginException(){}
      FailedLoginException(string m){
         whatstr = (char*)malloc(m.size()+1);
         unsigned int i;
         for(i = 0;i<m.size();i++){
            whatstr[i] = m[i];
         }
         whatstr[i] = '\0';
      }
      const char* what() const noexcept override{
         return whatstr;
      }
   private:
      char* whatstr;
   };
   };
}