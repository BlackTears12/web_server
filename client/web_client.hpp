#pragma once
#include "client_connection_resolver.hpp"

namespace web_client
{
   class Web_Client{
   public:
      static Web_Client* Instance();
      static void Destroy();
   private:
      static Web_Client* instance;
      Client_Connection_Resolver* connection_resolver;

      Web_Client();
      ~Web_Client();
   };
}