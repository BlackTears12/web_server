#include "web_client.hpp"

namespace web_client
{

   Web_Client* Web_Client::instance = nullptr;

   Web_Client* Web_Client::Instance(){
      if(!instance)
         instance = new Web_Client();
      return instance;
   }

   void Web_Client::Destroy(){
      if(instance)
         delete instance;
   }

   Web_Client::Web_Client(){
      connection_resolver = new Client_Connection_Resolver();
   }
   
   Web_Client::~Web_Client(){
      delete connection_resolver;
   }
}