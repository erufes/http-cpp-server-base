/* http-cpp-server-base
 * Base de um servidor http simples em c++ para escrever 
 * serviços para projetos de robótica.
 * Definition of the ServerSocket class
 * 
 * @author : Rob Tougher (mail-to: rtougher@yahoo.com)
 * @source : http://tldp.org/LDP/LG/issue74/tougher.html
 * @date : 2002
 */

#ifndef ServerSocket_class
#define ServerSocket_class

#include "Socket.h"


class ServerSocket : private Socket
{
 public:

  ServerSocket ( int port );
  ServerSocket (){};
  virtual ~ServerSocket();

  const ServerSocket& operator << ( const std::string& ) const;
  const ServerSocket& operator >> ( std::string& ) const;

  void accept ( ServerSocket& );

};


#endif