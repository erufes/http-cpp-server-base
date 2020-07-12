/* http-cpp-server-base
 * Base de um servidor http simples em c++ para escrever 
 * serviços para projetos de robótica.
 * SocketException class
 * 
 * @author : Rob Tougher (mail-to: rtougher@yahoo.com)
 * @source : http://tldp.org/LDP/LG/issue74/tougher.html
 * @date : 2002
 */

#ifndef SocketException_class
#define SocketException_class

#include <string>

class SocketException
{
 public:
  SocketException ( std::string s ) : m_s ( s ) {};
  ~SocketException (){};

  std::string description() { return m_s; }

 private:

  std::string m_s;

};

#endif