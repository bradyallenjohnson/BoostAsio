
#include <iostream>

#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include "common.hh"
#include "TcpServerMT.hh"

using namespace std;

int main(int argc, char **argv)
{
  cout << "ASync MultiThreaded server" << endl;

  uint16_t tcpListenPort;
  if(!getServerConfig(argc, argv, tcpListenPort))
  {
    return 1;
  }

  try
  {
    boost::asio::io_service io_service_acceptor;
    boost::asio::io_service io_service_comm;

    // this causes the io_service::run() to not stop even though currently there is nothing to do
    boost::asio::io_service::work work_acceptor(io_service_acceptor);
    boost::asio::io_service::work work_comm(io_service_comm);

    TcpServerMT server(io_service_acceptor, io_service_comm, tcpListenPort);
    server.startAccept();

    // Create a thread for the acceptor
    boost::thread accept_thread(boost::bind(&boost::asio::io_service::run, &io_service_acceptor));

    // Create a thread pool that will execute the io_service work/handlers, namely the
    // TcpServerMT connection accepting, and the TcpSession reads and writes
    boost::thread_group worker_threads;
    for(int i = 0; i < 3; ++i)
    {
      worker_threads.create_thread(boost::bind(&boost::asio::io_service::run, &io_service_comm));
    }

    // TODO need to "unassign" work_acceptor and work_comm to stop the threads

    accept_thread.join();
    worker_threads.join_all();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
