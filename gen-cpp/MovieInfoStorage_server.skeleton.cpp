// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "MovieInfoStorage.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::NetflixMicroservices;

class MovieInfoStorageHandler : virtual public MovieInfoStorageIf {
 public:
  MovieInfoStorageHandler() {
    // Your initialization goes here
  }

  void ping() {
    // Your implementation goes here
    printf("ping\n");
  }

  void get_info(std::string& _return, const std::string& req_id, const std::string& movie_id, const std::string& type) {
    // Your implementation goes here
    printf("get_info\n");
  }

};

int main(int argc, char **argv) {
  int port = 9090;
  shared_ptr<MovieInfoStorageHandler> handler(new MovieInfoStorageHandler());
  shared_ptr<TProcessor> processor(new MovieInfoStorageProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

