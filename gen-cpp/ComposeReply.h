/**
 * Autogenerated by Thrift Compiler (0.10.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef ComposeReply_H
#define ComposeReply_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "TwitterMicroservices_types.h"

namespace TwitterMicroservices {

#ifdef _WIN32
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class ComposeReplyIf {
 public:
  virtual ~ComposeReplyIf() {}
  virtual void ping() = 0;
  virtual void upload(const std::string& req_id, const std::string& type, const std::string& data) = 0;
};

class ComposeReplyIfFactory {
 public:
  typedef ComposeReplyIf Handler;

  virtual ~ComposeReplyIfFactory() {}

  virtual ComposeReplyIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(ComposeReplyIf* /* handler */) = 0;
};

class ComposeReplyIfSingletonFactory : virtual public ComposeReplyIfFactory {
 public:
  ComposeReplyIfSingletonFactory(const boost::shared_ptr<ComposeReplyIf>& iface) : iface_(iface) {}
  virtual ~ComposeReplyIfSingletonFactory() {}

  virtual ComposeReplyIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(ComposeReplyIf* /* handler */) {}

 protected:
  boost::shared_ptr<ComposeReplyIf> iface_;
};

class ComposeReplyNull : virtual public ComposeReplyIf {
 public:
  virtual ~ComposeReplyNull() {}
  void ping() {
    return;
  }
  void upload(const std::string& /* req_id */, const std::string& /* type */, const std::string& /* data */) {
    return;
  }
};


class ComposeReply_ping_args {
 public:

  ComposeReply_ping_args(const ComposeReply_ping_args&);
  ComposeReply_ping_args& operator=(const ComposeReply_ping_args&);
  ComposeReply_ping_args() {
  }

  virtual ~ComposeReply_ping_args() throw();

  bool operator == (const ComposeReply_ping_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const ComposeReply_ping_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ComposeReply_ping_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ComposeReply_ping_pargs {
 public:


  virtual ~ComposeReply_ping_pargs() throw();

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ComposeReply_ping_result {
 public:

  ComposeReply_ping_result(const ComposeReply_ping_result&);
  ComposeReply_ping_result& operator=(const ComposeReply_ping_result&);
  ComposeReply_ping_result() {
  }

  virtual ~ComposeReply_ping_result() throw();

  bool operator == (const ComposeReply_ping_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const ComposeReply_ping_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ComposeReply_ping_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ComposeReply_ping_presult {
 public:


  virtual ~ComposeReply_ping_presult() throw();

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _ComposeReply_upload_args__isset {
  _ComposeReply_upload_args__isset() : req_id(false), type(false), data(false) {}
  bool req_id :1;
  bool type :1;
  bool data :1;
} _ComposeReply_upload_args__isset;

class ComposeReply_upload_args {
 public:

  ComposeReply_upload_args(const ComposeReply_upload_args&);
  ComposeReply_upload_args& operator=(const ComposeReply_upload_args&);
  ComposeReply_upload_args() : req_id(), type(), data() {
  }

  virtual ~ComposeReply_upload_args() throw();
  std::string req_id;
  std::string type;
  std::string data;

  _ComposeReply_upload_args__isset __isset;

  void __set_req_id(const std::string& val);

  void __set_type(const std::string& val);

  void __set_data(const std::string& val);

  bool operator == (const ComposeReply_upload_args & rhs) const
  {
    if (!(req_id == rhs.req_id))
      return false;
    if (!(type == rhs.type))
      return false;
    if (!(data == rhs.data))
      return false;
    return true;
  }
  bool operator != (const ComposeReply_upload_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ComposeReply_upload_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ComposeReply_upload_pargs {
 public:


  virtual ~ComposeReply_upload_pargs() throw();
  const std::string* req_id;
  const std::string* type;
  const std::string* data;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

class ComposeReplyClient : virtual public ComposeReplyIf {
 public:
  ComposeReplyClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  ComposeReplyClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  void upload(const std::string& req_id, const std::string& type, const std::string& data);
  void send_upload(const std::string& req_id, const std::string& type, const std::string& data);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class ComposeReplyProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<ComposeReplyIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (ComposeReplyProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_ping(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_upload(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  ComposeReplyProcessor(boost::shared_ptr<ComposeReplyIf> iface) :
    iface_(iface) {
    processMap_["ping"] = &ComposeReplyProcessor::process_ping;
    processMap_["upload"] = &ComposeReplyProcessor::process_upload;
  }

  virtual ~ComposeReplyProcessor() {}
};

class ComposeReplyProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  ComposeReplyProcessorFactory(const ::boost::shared_ptr< ComposeReplyIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< ComposeReplyIfFactory > handlerFactory_;
};

class ComposeReplyMultiface : virtual public ComposeReplyIf {
 public:
  ComposeReplyMultiface(std::vector<boost::shared_ptr<ComposeReplyIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~ComposeReplyMultiface() {}
 protected:
  std::vector<boost::shared_ptr<ComposeReplyIf> > ifaces_;
  ComposeReplyMultiface() {}
  void add(boost::shared_ptr<ComposeReplyIf> iface) {
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

  void upload(const std::string& req_id, const std::string& type, const std::string& data) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->upload(req_id, type, data);
    }
    ifaces_[i]->upload(req_id, type, data);
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class ComposeReplyConcurrentClient : virtual public ComposeReplyIf {
 public:
  ComposeReplyConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  ComposeReplyConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  void upload(const std::string& req_id, const std::string& type, const std::string& data);
  void send_upload(const std::string& req_id, const std::string& type, const std::string& data);
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
