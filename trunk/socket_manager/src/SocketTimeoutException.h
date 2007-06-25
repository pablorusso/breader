// SocketTimeoutException class


#ifndef SocketTimeoutException_class
#define SocketTimeoutException_class

#include <string>

class SocketTimeoutException
{
 public:
  SocketTimeoutException () : m_s ( "Se agoto el tiempo de espera." ) {};
  ~SocketTimeoutException (){};

  std::string description() { return m_s; }

 private:

  std::string m_s;

};

#endif
