#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>
#include <string>
#include <thread>

using std::string;
using boost::asio::ip::address;
using boost::asio::io_context;
using tcp = boost::asio::ip::tcp;

void process_socket_connection(tcp::socket q){
   boost::beast::websocket::stream<tcp::socket> ws(std::move(q));
   ws.accept();
   while(true)
   {
      try{//living connection
         boost::beast::flat_buffer buffer;
         ws.read(buffer);
         string out = boost::beast::buffers_to_string(buffer.cdata());
         std::cout << out << std::endl;
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

int main(){

   address const address = boost::asio::ip::make_address("127.0.0.1");
   unsigned short port = static_cast<unsigned short>(std::atoi("8083"));

   io_context ioc(1);   //is same as io_context ioc{1}
   tcp::acceptor acceptor(ioc,{address,port});

   while(true){
      tcp::socket socket(ioc);
      acceptor.accept(socket);
      std::cout << "socket accepted" << std::endl;

      //spawning a thread to work with accepted socket
      std::thread(process_socket_connection,std::move(socket)).detach();
   }
   return 0;
}