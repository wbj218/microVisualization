/**
 * Autogenerated by Thrift Compiler (0.10.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef ComposeReview_H
#define ComposeReview_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "NetflixMicroservices_types.h"

namespace NetflixMicroservices {

#ifdef _WIN32
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class ComposeReviewIf {
 public:
  virtual ~ComposeReviewIf() {}
  virtual void ping() = 0;
  virtual void upload(const std::string& req_id, const std::string& user_id, const std::string& type, const std::string& data) = 0;
};

class ComposeReviewIfFactory {
 public:
  typedef ComposeReviewIf Handler;

  virtual ~ComposeReviewIfFactory() {}

  virtual ComposeReviewIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(ComposeReviewIf* /* handler */) = 0;
};

class ComposeReviewIfSingletonFactory : virtual public ComposeReviewIfFactory {
 public:
  ComposeReviewIfSingletonFactory(const boost::shared_ptr<ComposeReviewIf>& iface) : iface_(iface) {}
  virtual ~ComposeReviewIfSingletonFactory() {}

  virtual ComposeReviewIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(ComposeReviewIf* /* handler */) {}

 protected:
  boost::shared_ptr<ComposeReviewIf> iface_;
};

class ComposeReviewNull : virtual public ComposeReviewIf {
 public:
  virtual ~ComposeReviewNull() {}
  void ping() {
    return;
  }
  void upload(const std::string& /* req_id */, const std::string& /* user_id */, const std::string& /* type */, const std::string& /* data */) {
    return;
  }
};


class ComposeReview_ping_args {
 public:

  ComposeReview_ping_args(const ComposeReview_ping_args&);
  ComposeReview_ping_args& operator=(const ComposeReview_ping_args&);
  ComposeReview_ping_args() {
  }

  virtual ~ComposeReview_ping_args() throw();

  bool operator == (const ComposeReview_ping_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const ComposeReview_ping_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ComposeReview_ping_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ComposeReview_ping_pargs {
 public:


  virtual ~ComposeReview_ping_pargs() throw();

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ComposeReview_ping_result {
 public:

  ComposeReview_ping_result(const ComposeReview_ping_result&);
  ComposeReview_ping_result& operator=(const ComposeReview_ping_result&);
  ComposeReview_ping_result() {
  }

  virtual ~ComposeReview_ping_result() throw();

  bool operator == (const ComposeReview_ping_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const ComposeReview_ping_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ComposeReview_ping_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ComposeReview_ping_presult {
 public:


  virtual ~ComposeReview_ping_presult() throw();

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _ComposeReview_upload_args__isset {
  _ComposeReview_upload_args__isset() : req_id(false), user_id(false), type(false), data(false) {}
  bool req_id :1;
  bool user_id :1;
  bool type :1;
  bool data :1;
} _ComposeReview_upload_args__isset;

class ComposeReview_upload_args {
 public:

  ComposeReview_upload_args(const ComposeReview_upload_args&);
  ComposeReview_upload_args& operator=(const ComposeReview_upload_args&);
  ComposeReview_upload_args() : req_id(), user_id(), type(), data() {
  }

  virtual ~ComposeReview_upload_args() throw();
  std::string req_id;
  std::string user_id;
  std::string type;
  std::string data;

  _ComposeReview_upload_args__isset __isset;

  void __set_req_id(const std::string& val);

  void __set_user_id(const std::string& val);

  void __set_type(const std::string& val);

  void __set_data(const std::string& val);

  bool operator == (const ComposeReview_upload_args & rhs) const
  {
    if (!(req_id == rhs.req_id))
      return false;
    if (!(user_id == rhs.user_id))
      return false;
    if (!(type == rhs.type))
      return false;
    if (!(data == rhs.data))
      return false;
    return true;
  }
  bool operator != (const ComposeReview_upload_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ComposeReview_upload_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ComposeReview_upload_pargs {
 public:


  virtual ~ComposeReview_upload_pargs() throw();
  const std::string* req_id;
  const std::string* user_id;
  const std::string* type;
  const std::string* data;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

class ComposeReviewClient : virtual public ComposeReviewIf {
 public:
  ComposeReviewClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  ComposeReviewClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  void upload(const std::string& req_id, const std::string& user_id, const std::string& type, const std::string& data);
  void send_upload(const std::string& req_id, const std::string& user_id, const std::string& type, const std::string& data);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class ComposeReviewProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<ComposeReviewIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (ComposeReviewProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_ping(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_upload(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  ComposeReviewProcessor(boost::shared_ptr<ComposeReviewIf> iface) :
    iface_(iface) {
    processMap_["ping"] = &ComposeReviewProcessor::process_ping;
    processMap_["upload"] = &ComposeReviewProcessor::process_upload;
  }

  virtual ~ComposeReviewProcessor() {}
};

class ComposeReviewProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  ComposeReviewProcessorFactory(const ::boost::shared_ptr< ComposeReviewIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< ComposeReviewIfFactory > handlerFactory_;
};

class ComposeReviewMultiface : virtual public ComposeReviewIf {
 public:
  ComposeReviewMultiface(std::vector<boost::shared_ptr<ComposeReviewIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~ComposeReviewMultiface() {}
 protected:
  std::vector<boost::shared_ptr<ComposeReviewIf> > ifaces_;
  ComposeReviewMultiface() {}
  void add(boost::shared_ptr<ComposeReviewIf> iface) {
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

  void upload(const std::string& req_id, const std::string& user_id, const std::string& type, const std::string& data) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->upload(req_id, user_id, type, data);
    }
    ifaces_[i]->upload(req_id, user_id, type, data);
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class ComposeReviewConcurrentClient : virtual public ComposeReviewIf {
 public:
  ComposeReviewConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  ComposeReviewConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  void upload(const std::string& req_id, const std::string& user_id, const std::string& type, const std::string& data);
  void send_upload(const std::string& req_id, const std::string& user_id, const std::string& type, const std::string& data);
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
