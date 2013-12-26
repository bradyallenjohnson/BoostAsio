
#include <string>
#include <iostream>

#include <boost/bind.hpp>

#include "common.hh"
#include "TcpClient.hh"

using namespace boost::asio::ip;
using namespace std;

// private
TcpClient::TcpClient(boost::asio::io_service& io_service, uint16_t tcpConnectPort, std::ifstream &infile) :
        socket_(io_service),
        tcpConnectPort_(tcpConnectPort),
        infile_(infile)
{
}

TcpClient::~TcpClient()
{
  cout << "Destroying TcpClient: " << socket_.remote_endpoint() << endl;
}

// static method
TcpClient::shared_pointer TcpClient::create(boost::asio::io_service& io_service, uint16_t tcpConnectPort, std::ifstream &infile)
{
  return shared_pointer(new TcpClient(io_service, tcpConnectPort, infile));
}

void TcpClient::start()
{
  // performs a Connect
  socket_.async_connect(tcp::endpoint(tcp::v4(), tcpConnectPort_),
     boost::bind(&TcpClient::handleConnect, this,
                 boost::asio::placeholders::error));
}

void TcpClient::handleConnect(const boost::system::error_code& error)
{
  if(error)
  {
    cout << "Error connecting to server: " << error << endl;
    return;
  }

  cout << "Connected to Server: " << socket_.remote_endpoint() << endl;

  // A connection was accepted, now kick-off the first write
  if(!getNextLine())
  {
    cout << "Empty file" << endl;
    return;
  }

  boost::asio::async_write(socket_, boost::asio::buffer(line_),
      boost::bind(&TcpClient::handleWrite, shared_from_this(),
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
}

void TcpClient::handleWrite(const boost::system::error_code& error, size_t bytes_transferred)
{
  // A write was performed, now kick-off a/another read

  if(error == boost::asio::error::eof)
  {
    cout << "TcpClient::handleWrite: Server disconnected" << endl;
    // connection closed cleanly by peer
    // Returning will cause this TcpClient to be destroyed by the shared_ptr
    // since no-one else has a reference to it
    return;
  }

  cout << "TcpClient::handleWrite: errorCode " << error << " bytes written " << bytes_transferred << endl;

  if(!error)
  {
    cout << "\trequest:  " << line_ << endl;
    socket_.async_read_some(boost::asio::buffer(data_, maxLength),
        boost::bind(&TcpClient::handleRead, shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
  }
  else
  {
    throw boost::system::system_error(error);
  }
}

void TcpClient::handleRead(const boost::system::error_code& error, size_t bytes_transferred)
{
  // A read was performed, now prepare the response and kick-off a write to send it

  if(error == boost::asio::error::eof)
  {
    cout << "TcpClient::handleRead: Server disconnected" << endl;
    // connection closed cleanly by peer
    // Returning will cause this TcpClient to be destroyed by the shared_ptr
    // since no-one else has a reference to it
    return;
  }

  cout << "TcpClient::handleRead: errorCode " << error << " bytes read " << bytes_transferred << endl;

  if(!error)
  {
    string request(data_, bytes_transferred);
    cout << "\tresponse: " << request << endl;
    if(getNextLine())
    {
      boost::asio::async_write(socket_, boost::asio::buffer(line_),
          boost::bind(&TcpClient::handleWrite,
                      shared_from_this(),
                      boost::asio::placeholders::error,
                      boost::asio::placeholders::bytes_transferred));
    }
  }
  else
  {
    throw boost::system::system_error(error);
  }
}

bool TcpClient::getNextLine()
{
  if(getline(infile_, line_))
  {
    return true;
  }
  else
  {
    return false;
  }
}

