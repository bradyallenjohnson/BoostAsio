
#include <iostream>
#include <string>
#include <fstream>
#include <boost/asio.hpp>

#include "common.hh"

using namespace std;
using namespace boost::asio::ip;

void exchangeData(tcp::socket &socket, ifstream &infile)
{
  string line;
  while(getline(infile, line))
  {
    char data[maxLength];
    boost::system::error_code error;

    cout << "Client writing to server:    " << line << endl;
    boost::asio::write(socket, boost::asio::buffer(line));
    size_t length = socket.read_some(boost::asio::buffer(data), error);
    if(error == boost::asio::error::eof)
    {
      cout << "Server disconnected while reading from it" << endl;
      break; // connection closed cleanly by peer
    }
    else if(error)
    {
      throw boost::system::system_error(error);
    }

    string response(data, length);
    cout << "Client received from server: " << response << endl;
  }
}

void client(boost::asio::io_service &io_service, uint16_t tcpPort, const string &infileName)
{
  // Connect to the server
  // For each line in infile:
  // - Send the line
  // - Wait for the response
  try
  {
    ifstream infile(infileName.c_str());
    // Dont try to connect if the file cant be opened
    if(!infile)
    {
      cerr << "Unable to open infile: " << infileName << endl;
      return;
    }

    tcp::socket socket(io_service);
    socket.connect(tcp::endpoint(tcp::v4(), tcpPort)); // blocks until accepted by server
    cout << "Connected to server" << endl;

    exchangeData(socket, infile);
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

int main(int argc, char **argv)
{
  cout << "Sync client" << endl;

  string infileName;
  uint16_t tcpConnectPort;
  if(!getClientConfig(argc, argv, tcpConnectPort, infileName))
  {
    return 1;
  }

  try
  {
    boost::asio::io_service io_service;
    client(io_service, tcpConnectPort, infileName);
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
