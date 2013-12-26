
#include <string>
#include <utility> // swap()
#include <boost/program_options.hpp>

using namespace std;
using namespace boost;

const uint16_t DEFAULT_TCP_PORT = 44444;
const string DEFAULT_INFILE = "data.txt";

void reverseWords(const string &in, string &out)
{
  // First copy the input string to the output string in reverse order
  for(int i = in.length()-1; i>=0; i--)
  {
    out.push_back(in[i]);
  }

  // Then reverse each word
  int start = 0;
  int end = out.find(' '); // TODO need to also consider tabs, etc

  while(end != string::npos)
  {
    // Reverse the word
    for(int i = 0; i < (end-start)/2; i++)
    {
      swap(out[i+start], out[end-1-i]);
    }

    // get the next word
    start = end+1;
    end = out.find(' ', start); // TODO need to also consider tabs, etc
    if(end == string::npos && start != out.length()+1)
    {
      end = out.length();
    }
  }
}

bool getClientConfig(int argc, char **argv, uint16_t &tcpPort, std::string &infile)
{
  program_options::options_description options("Options");
  options.add_options()
     ("help,h", "produce help message")
     ("input-file,f", program_options::value<std::string>(&infile)->default_value(DEFAULT_INFILE),
      "Input file with text to send to echo server")
     ("tcp_port,p", program_options::value<uint16_t>(&tcpPort)->default_value(DEFAULT_TCP_PORT),
      "The TCP port of the server to connect to")
     ;

  program_options::variables_map vm;
  program_options::store(program_options::parse_command_line(argc, argv, options), vm);
  notify(vm);

  return true;
}

bool getServerConfig(int argc, char **argv, uint16_t &tcpPort)
{
  program_options::options_description options("Options");
  options.add_options()
     ("help,h", "produce help message")
     ("tcp_port,p", program_options::value<uint16_t>(&tcpPort)->default_value(DEFAULT_TCP_PORT),
      "The TCP port to listen on")
     ;

  program_options::variables_map vm;
  program_options::store(program_options::parse_command_line(argc, argv, options), vm);
  notify(vm);

  return true;
}

