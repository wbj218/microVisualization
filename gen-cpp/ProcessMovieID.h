/**
 * Autogenerated by Thrift Compiler (0.10.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef ProcessMovieID_H
#define ProcessMovieID_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "NetflixMicroservices_types.h"

namespace NetflixMicroservices {

#ifdef _WIN32
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class ProcessMovieIDIf {
 public:
  virtual ~ProcessMovieIDIf() {}
  virtual void ping() = 0;
  virtual void get_movie_id(const std::string& req_id, const std::string& url) = 0;
};

class ProcessMovieIDIfFactory {
 public:
  typedef ProcessMovieIDIf Handler;

  virtual ~ProcessMovieIDIfFactory() {}

  virtual ProcessMovieIDIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(ProcessMovieIDIf* /* handler */) = 0;
};

class ProcessMovieIDIfSingletonFactory : virtual public ProcessMovieIDIfFactory {
 public:
  ProcessMovieIDIfSingletonFactory(const boost::shared_ptr<ProcessMovieIDIf>& iface) : iface_(iface) {}
  virtual ~ProcessMovieIDIfSingletonFactory() {}

  virtual ProcessMovieIDIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(ProcessMovieIDIf* /* handler */) {}

 protected:
  boost::shared_ptr<ProcessMovieIDIf> iface_;
};

class ProcessMovieIDNull : virtual public ProcessMovieIDIf {
 public:
  virtual ~ProcessMovieIDNull() {}
  void ping() {
    return;
  }
  void get_movie_id(const std::string& /* req_id */, const std::string& /* url */) {
    return;
  }
};


class ProcessMovieID_ping_args {
 public:

  ProcessMovieID_ping_args(const ProcessMovieID_ping_args&);
  ProcessMovieID_ping_args& operator=(const ProcessMovieID_ping_args&);
  ProcessMovieID_ping_args() {
  }

  virtual ~ProcessMovieID_ping_args() throw();

  bool operator == (const ProcessMovieID_ping_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const ProcessMovieID_ping_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ProcessMovieID_ping_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ProcessMovieID_ping_pargs {
 public:


  virtual ~ProcessMovieID_ping_pargs() throw();

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ProcessMovieID_ping_result {
 public:

  ProcessMovieID_ping_result(const ProcessMovieID_ping_result&);
  ProcessMovieID_ping_result& operator=(const ProcessMovieID_ping_result&);
  ProcessMovieID_ping_result() {
  }

  virtual ~ProcessMovieID_ping_result() throw();

  bool operator == (const ProcessMovieID_ping_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const ProcessMovieID_ping_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ProcessMovieID_ping_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ProcessMovieID_ping_presult {
 public:


  virtual ~ProcessMovieID_ping_presult() throw();

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _ProcessMovieID_get_movie_id_args__isset {
  _ProcessMovieID_get_movie_id_args__isset() : req_id(false), url(false) {}
  bool req_id :1;
  bool url :1;
} _ProcessMovieID_get_movie_id_args__isset;

class ProcessMovieID_get_movie_id_args {
 public:

  ProcessMovieID_get_movie_id_args(const ProcessMovieID_get_movie_id_args&);
  ProcessMovieID_get_movie_id_args& operator=(const ProcessMovieID_get_movie_id_args&);
  ProcessMovieID_get_movie_id_args() : req_id(), url() {
  }

  virtual ~ProcessMovieID_get_movie_id_args() throw();
  std::string req_id;
  std::string url;

  _ProcessMovieID_get_movie_id_args__isset __isset;

  void __set_req_id(const std::string& val);

  void __set_url(const std::string& val);

  bool operator == (const ProcessMovieID_get_movie_id_args & rhs) const
  {
    if (!(req_id == rhs.req_id))
      return false;
    if (!(url == rhs.url))
      return false;
    return true;
  }
  bool operator != (const ProcessMovieID_get_movie_id_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ProcessMovieID_get_movie_id_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ProcessMovieID_get_movie_id_pargs {
 public:


  virtual ~ProcessMovieID_get_movie_id_pargs() throw();
  const std::string* req_id;
  const std::string* url;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

class ProcessMovieIDClient : virtual public ProcessMovieIDIf {
 public:
  ProcessMovieIDClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  ProcessMovieIDClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void ping();
  void send_ping();
  void recv_ping();
  void get_movie_id(const std::string& req_id, const std::string& url);
  void send_get_movie_id(const std::string& req_id, const std::string& url);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class ProcessMovieIDProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<ProcessMovieIDIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (ProcessMovieIDProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_ping(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_get_movie_id(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  ProcessMovieIDProcessor(boost::shared_ptr<ProcessMovieIDIf> iface) :
    iface_(iface) {
    processMap_["ping"] = &ProcessMovieIDProcessor::process_ping;
    processMap_["get_movie_id"] = &ProcessMovieIDProcessor::process_get_movie_id;
  }

  virtual ~ProcessMovieIDProcessor() {}
};

class ProcessMovieIDProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  ProcessMovieIDProcessorFactory(const ::boost::shared_ptr< ProcessMovieIDIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< ProcessMovieIDIfFactory > handlerFactory_;
};

class ProcessMovieIDMultiface : virtual public ProcessMovieIDIf {
 public:
  ProcessMovieIDMultiface(std::vector<boost::shared_ptr<ProcessMovieIDIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~ProcessMovieIDMultiface() {}
 protected:
  std::vector<boost::shared_ptr<ProcessMovieIDIf> > ifaces_;
  ProcessMovieIDMultiface() {}
  void add(boost::shared_ptr<ProcessMovieIDIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void ping() {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->ping();
    }
    ifaces_[i]->ping();
  }

  void get_movie_id(const std::string& req_id, const std::string& url) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->get_movie_id(req_id, url);
    }
    ifaces_[i]->get_movie_id(req_id, url);
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class ProcessMovieIDConcurrentClient : virtual public ProcessMovieIDIf {
 public:
  ProcessMovieIDConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  ProcessMovieIDConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void ping();
  int32_t send_ping();
  void recv_ping(const int32_t seqid);
  void get_movie_id(const std::string& req_id, const std::string& url);
  void send_get_movie_id(const std::string& req_id, const std::string& url);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
  ::apache::thrift::async::TConcurrentClientSyncInfo sync_;
};

#ifdef _WIN32
  #pragma warning( pop )
#endif

} // namespace

#endif
