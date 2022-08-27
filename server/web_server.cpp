#include "web_server.hpp"
namespace web_server
{
   Web_Server* Web_Server::instance = nullptr;

   Web_Server* Web_Server::Instance(){
      if(!instance)
         instance = new Web_Server();
      return instance;
   }

   void Web_Server::Destroy(){
      if(instance)
         delete instance;
   }

   void Web_Server::process_socket_connection(tcp::socket q){
      websocket::stream<tcp::socket> ws(std::move(q));
      ws.accept();
      User* user;
      try{
         user = login(ws);
      }
      catch(std::exception &e){
         std::cerr << "failed login "<<e.what();
         ws.write(net::buffer("failed login "+string(e.what())));
         return;
      }
      ws.write(net::buffer("login ok"));
      while(true)
      {
         try{//living connection
            beast::flat_buffer buffer;
            ws.read(buffer);
            process_request(user,buffer);
            ws.write(buffer.data());
         }
         catch(beast::system_error const& e){
            if(e.code() != websocket::error::closed){
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

   void Web_Server::parse_login_buffer(beast::flat_buffer &buffer,login_data &data){
      string buff_data = beast::buffers_to_string(buffer.data());
      int i = 0;
      while(buff_data[i] != '-' && i < buff_data.size()){
         data.username += buff_data[i];
         ++i;
      }
      data.passwHash = buff_data.substr(i);
   }

   User* Web_Server::login(websocket::stream<tcp::socket> &ws){
      ws.write(net::buffer("<login_request>"));
      beast::flat_buffer buff;
      ws.read(buff);
      login_data data;
      parse_login_buffer(buff,data);
      User* usr = user_containter->get_user(data.username);
      if(!usr)
         throw FailedLoginException("no user found");
      if(hash(usr->get_privatekey(),data.passwHash) != usr->get_passwHash());
         throw FailedLoginException("wrong password");
   }
}