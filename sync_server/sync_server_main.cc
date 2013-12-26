
#include <iostream>
#include <boost/asio.hpp>

#include <common.hh>

using namespace std;
using namespace boost::asio::ip;

void session(tcp::socket &socket)
{
  try
  {
    for(;;) // loop until EOF received from client
    {
      char data[maxLength];
      boost::system::error_code error;
      size_t length = socket.read_some(boost::asio::buffer(data), error);
      string request(data, length);

      if(error == boost::asio::error::eof)
      {
        cout << "Client disconnected from server" << endl;
        break; // connection closed cleanly by peer
      }
      else if(error)
      {
        throw boost::system::system_error(error);
      }

      string response;
      reverseWords(request, response);
      cout << "Server received: " << request << endl;
      cout << "Server response: " << response << endl;

      boost::asio::write(socket, boost::asio::buffer(response, length));
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

void server(boost::asio::io_service &io_service, uint16_t tcpPort)
{
  try
  {
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), tcpPort));

    // For each client that connects:
    // - Accept the connection
    // - Read from the client until EOF received
    // - Reply to each client request with a response.
    // If multiple clients connect simultaneously, the second client
    // wont be attended until finished with the first client
    for (;;)
    {
      tcp::socket socket(io_service);
      tcp::endpoint remote;
      acceptor.accept(socket, remote);
      cout << "Accepted a client connection from: " << remote << endl;

      session(socket); // TODO could make this call in a separate thread
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

int main(int argc, char **argv)
{
  cout << "Sync server" << endl;
  uint16_t tcpListenPort;
  if(!getServerConfig(argc, argv, tcpListenPort))
  {
    return 1;
  }

  try
  {
    boost::asio::io_service io_service;
    server(io_service, tcpListenPort);
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
