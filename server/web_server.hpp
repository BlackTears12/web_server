#pragma once
#include "user_container.hpp"
#include "requests.hpp"
#include "../common/hash.hpp"
using namespace common;

namespace web_server
{
   struct login_data{
      string username;
      string passwHash;
   };

   class Web_Server{
   public:
      static Web_Server* Instance();
      static void Delete();
      void process_socket_connection(tcp::socket q);
   private:
      static Web_Server* instance;
      User_Container* user_containter;
   
      Web_Server();
      ~Web_Server();
      void process_request(User* sender,beast::flat_buffer &request);
      void send_message(User* sender,vector<string> args);
      void parse_login_buffer(beast::websocket::stream<tcp::socket> &ws,login_data &data);
      User* login(beast::websocket::stream<tcp::socket> &ws);

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

   void parse_login_buffer(beast::websocket::stream<tcp::socket> &ws,login_data &data){
      
   }

   User* Web_Server::login(beast::websocket::stream<tcp::socket> &ws){
      ws.write("<login_request>");
      beast::flat_buffer buff;
      ws.read(buff);
      login_data data;
      parse_login_buffer(ws,data);
      User* usr = user_containter->get_user(data.username);
      if(!usr)
         throw FailedLoginException("no user found");
      if(hash(usr->get_privatekey(),data.passwHash) != usr->get_passwHash());
         throw FailedLoginException("wrong password");
   }

   void Web_Server::process_request(User* sender,beast::flat_buffer &request){
      Request r(sender,request);
      switch (r.type)
      {
      case Request::MESSAGE:
         send_message(sender,r.args);
         break;

      default:
         break;
      }
   }

   void Web_Server::process_socket_connection(tcp::socket q){
      boost::beast::websocket::stream<tcp::socket> ws(std::move(q));
      ws.accept();
      User* user;
      try{
         user = login(ws);
      }
      catch(std::exception &e){
         std::cerr << "failed login "<<e.what();
         ws.write("failed login "+string(e.what()));
         return;
      }
      ws.write("login ok");
      while(true)
      {
         try{//living connection
            boost::beast::flat_buffer buffer;
            ws.read(buffer);
            process_request(user,buffer);
            ws.write(buffer.data());
         }
         catch(boost::beast::system_error const& e){
            if(e.code() != boost::beast::websocket::error::closed){
               std::cout << e.code().message() << std::endl;
               break;
            }
         }
      }
   }
}