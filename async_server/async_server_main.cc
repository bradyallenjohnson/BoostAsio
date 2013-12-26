
#include <iostream>

#include "common.hh"
#include "TcpServer.hh"

using namespace std;

int main(int argc, char **argv)
{
  cout << "ASync server" << endl;

  uint16_t tcpListenPort;
  if(!getServerConfig(argc, argv, tcpListenPort))
  {
    return 1;
  }

  try
  {
    boost::asio::io_service io_service;
    TcpServer server(io_service, tcpListenPort);
    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
