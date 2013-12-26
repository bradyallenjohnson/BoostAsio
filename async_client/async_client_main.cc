
#include <iostream>
#include <boost/asio.hpp>

#include "common.hh"
#include "TcpClient.hh"

using namespace std;

int main(int argc, char **argv)
{
  cout << "ASync client" << endl;

  string infileName;
  uint16_t tcpConnectPort;
  if(!getClientConfig(argc, argv, tcpConnectPort, infileName))
  {
    return 1;
  }

  ifstream infile(infileName.c_str());
  // Dont try to connect if the file cant be opened
  if(!infile)
  {
    cerr << "Unable to open infile: " << infileName << endl;
    return 1;
  }

  try
  {
      boost::asio::io_service io_service;
      TcpClient::shared_pointer client = TcpClient::create(io_service, tcpConnectPort, infile);
      client->start();

      io_service.run();
  }
  catch(std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
