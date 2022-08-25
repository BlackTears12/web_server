#include "web_server.hpp"
namespace web_server
{
   Web_Server* Web_Server::Instance(){
      if(!instance)
         instance = new Web_Server();
      return instance;
   }

   void Web_Server::Delete(){
      if(instance)
         delete instance;
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

   Web_Server::Web_Server(){

   }

   Web_Server::~Web_Server(){

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

   void Web_Server::send_message(User* sender,vector<string> args){

   }

   void Web_Server::parse_login_buffer(beast::websocket::stream<tcp::socket> &ws,login_data &data){

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
}