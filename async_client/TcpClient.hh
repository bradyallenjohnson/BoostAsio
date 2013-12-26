
#include <string>
#include <fstream>

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

class TcpClient : public boost::enable_shared_from_this<TcpClient>
{
public:
  typedef boost::shared_ptr<TcpClient> shared_pointer;
  ~TcpClient();

  static shared_pointer create(boost::asio::io_service& io_service, uint16_t tcpConnectPort, std::ifstream &infile);
  void start();

  inline boost::asio::ip::tcp::socket& socket() { return socket_; }

private:
  TcpClient(); // Explicitly disallowing default ctor
  TcpClient(boost::asio::io_service& io_service, uint16_t tcpConnectPort, std::ifstream &infile);
  bool getNextLine();
  void handleConnect(const boost::system::error_code& error);
  void handleRead(const boost::system::error_code& error, size_t bytes_transferred);
  void handleWrite(const boost::system::error_code& error, size_t bytes_transferred);

  boost::asio::ip::tcp::socket socket_;
  uint16_t tcpConnectPort_;
  std::string line_;
  char data_[maxLength];
  std::ifstream &infile_;

};

