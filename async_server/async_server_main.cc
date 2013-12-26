
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

    // Since io_service.run() will exit when all handlers complete, we could
    // do the following to make it not stop until explicitly told to stop
    //boost::optional<boost::asio::io_service::work> work = boost::in_place(boost::ref(io_service));
    // This will also cause it to stop: work = boost::none;
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
