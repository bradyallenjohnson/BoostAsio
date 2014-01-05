
#include <stdint.h>
#include <string>

const int maxLength = 1024;

void reverseWords(const std::string &in, std::string &out);
bool getClientConfig(int argc, char **argv, uint16_t &tcpPort, std::string &infile);
bool getServerConfig(int argc, char **argv, uint16_t &tcpPort);
