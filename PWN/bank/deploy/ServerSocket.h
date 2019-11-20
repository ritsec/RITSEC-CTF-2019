// Definition of the ServerSocket class

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
  const ServerSocket& operator << ( const long& i ) const;
  const ServerSocket& operator >> ( std::string& ) const;
  const ServerSocket& operator >> ( int& ) const;
  const ServerSocket& operator >> ( long& i ) const;

  void accept ( ServerSocket& );

};


#endif
