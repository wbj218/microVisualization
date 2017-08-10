// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "GetRating.h"
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

class GetRatingHandler : virtual public GetRatingIf {
 public:
  GetRatingHandler() {
    // Your initialization goes here
  }

  void ping() {
    // Your implementation goes here
    printf("ping\n");
  }

  void get_rating(const std::string& req_id, const std::string& movie_id, const int32_t server_no) {
    // Your implementation goes here
    printf("get_rating\n");
  }

};

int main(int argc, char **argv) {
  int port = 9090;
  shared_ptr<GetRatingHandler> handler(new GetRatingHandler());
  shared_ptr<TProcessor> processor(new GetRatingProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

