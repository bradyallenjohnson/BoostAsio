
#include <string>

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

//
// TcpSession
//

class TcpSession : public boost::enable_shared_from_this<TcpSession>
{
public:
  typedef boost::shared_ptr<TcpSession> shared_pointer;
  ~TcpSession();

  static shared_pointer create(boost::asio::io_service& io_service);

  inline boost::asio::ip::tcp::socket& socket() { return socket_; }

  void start();

private:
  TcpSession(boost::asio::io_service &io_service);

  void handleRead(const boost::system::error_code& error, size_t bytes_transferred);
  void handleWrite(const boost::system::error_code& error, size_t bytes_transferred);

  boost::asio::ip::tcp::socket socket_;
  std::string message_;
  char data_[maxLength];
};

//
// TcpServer
//

class TcpServer
{
public:
  TcpServer(boost::asio::io_service& io_service, uint16_t tcpListenPort);

private:
  void startAccept();
  void handleAccept(TcpSession::shared_pointer new_connection, const boost::system::error_code& error);

  boost::asio::ip::tcp::acceptor acceptor_;
};

