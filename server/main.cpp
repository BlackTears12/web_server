#include "web_server.hpp"
#include "../common/web_connection.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <sstream>

using namespace web_server;

int main(){
   address const address = boost::asio::ip::make_address("127.0.0.1");
   unsigned short port = static_cast<unsigned short>(std::atoi("8083"));

   io_context ioc(1);   //is same as io_context ioc{1}
   tcp::acceptor acceptor(ioc,{address,port});
   Web_Server* server = Web_Server::Instance();

   while(true){
      tcp::socket socket(ioc);
      acceptor.accept(socket);
      std::cout << "socket accepted" << std::endl;

      //spawning a thread to work with accepted socket
      std::thread(&Web_Server::process_socket_connection,server,std::move(socket)).detach();
   }
   return 0;
}