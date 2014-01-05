
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

  static shared_pointer create(boost::asio::io_service& io_service, int sessionId);

  inline boost::asio::ip::tcp::socket& socket() { return socket_; }

  void start();

private:
  TcpSession(boost::asio::io_service &io_service, int sessionId);

  void handleRead(const boost::system::error_code& error, size_t bytes_transferred);
  void handleWrite(const boost::system::error_code& error, size_t bytes_transferred);

  boost::asio::ip::tcp::socket socket_;
  int sessionId_;
  std::string message_;
  char data_[maxLength];
};

//
// TcpServerMT
//

class TcpServerMT
{
public:
  TcpServerMT(boost::asio::io_service& io_service_acceptor,
              boost::asio::io_service& io_service_comm,
	      uint16_t tcpListenPort);
  void startAccept();

private:
  void handleAccept(TcpSession::shared_pointer new_connection, const boost::system::error_code& error);

  boost::asio::io_service &io_service_comm_;
  boost::asio::ip::tcp::acceptor acceptor_;
};

