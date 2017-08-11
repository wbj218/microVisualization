<?php
namespace NetflixMicroservices;
/**
 * Autogenerated by Thrift Compiler (0.10.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
use Thrift\Base\TBase;
use Thrift\Type\TType;
use Thrift\Type\TMessageType;
use Thrift\Exception\TException;
use Thrift\Exception\TProtocolException;
use Thrift\Protocol\TProtocol;
use Thrift\Protocol\TBinaryProtocolAccelerated;
use Thrift\Exception\TApplicationException;


interface GetWatchNextIf {
  /**
   */
  public function ping();
  /**
   * @param string $req_id
   * @param string $user_id
   * @param int $server_no
   */
  public function get_watch_next($req_id, $user_id, $server_no);
}


class GetWatchNextClient implements \NetflixMicroservices\GetWatchNextIf {
  protected $input_ = null;
  protected $output_ = null;

  protected $seqid_ = 0;

  public function __construct($input, $output=null) {
    $this->input_ = $input;
    $this->output_ = $output ? $output : $input;
  }

  public function ping()
  {
    $this->send_ping();
    $this->recv_ping();
  }

  public function send_ping()
  {
    $args = new \NetflixMicroservices\GetWatchNext_ping_args();
    $bin_accel = ($this->output_ instanceof TBinaryProtocolAccelerated) && function_exists('thrift_protocol_write_binary');
    if ($bin_accel)
    {
      thrift_protocol_write_binary($this->output_, 'ping', TMessageType::CALL, $args, $this->seqid_, $this->output_->isStrictWrite());
    }
    else
    {
      $this->output_->writeMessageBegin('ping', TMessageType::CALL, $this->seqid_);
      $args->write($this->output_);
      $this->output_->writeMessageEnd();
      $this->output_->getTransport()->flush();
    }
  }

  public function recv_ping()
  {
    $bin_accel = ($this->input_ instanceof TBinaryProtocolAccelerated) && function_exists('thrift_protocol_read_binary');
    if ($bin_accel) $result = thrift_protocol_read_binary($this->input_, '\NetflixMicroservices\GetWatchNext_ping_result', $this->input_->isStrictRead());
    else
    {
      $rseqid = 0;
      $fname = null;
      $mtype = 0;

      $this->input_->readMessageBegin($fname, $mtype, $rseqid);
      if ($mtype == TMessageType::EXCEPTION) {
        $x = new TApplicationException();
        $x->read($this->input_);
        $this->input_->readMessageEnd();
        throw $x;
      }
      $result = new \NetflixMicroservices\GetWatchNext_ping_result();
      $result->read($this->input_);
      $this->input_->readMessageEnd();
    }
    return;
  }

  public function get_watch_next($req_id, $user_id, $server_no)
  {
    $this->send_get_watch_next($req_id, $user_id, $server_no);
  }

  public function send_get_watch_next($req_id, $user_id, $server_no)
  {
    $args = new \NetflixMicroservices\GetWatchNext_get_watch_next_args();
    $args->req_id = $req_id;
    $args->user_id = $user_id;
    $args->server_no = $server_no;
    $bin_accel = ($this->output_ instanceof TBinaryProtocolAccelerated) && function_exists('thrift_protocol_write_binary');
    if ($bin_accel)
    {
      thrift_protocol_write_binary($this->output_, 'get_watch_next', TMessageType::ONEWAY, $args, $this->seqid_, $this->output_->isStrictWrite());
    }
    else
    {
      $this->output_->writeMessageBegin('get_watch_next', TMessageType::ONEWAY, $this->seqid_);
      $args->write($this->output_);
      $this->output_->writeMessageEnd();
      $this->output_->getTransport()->flush();
    }
  }
}


// HELPER FUNCTIONS AND STRUCTURES

class GetWatchNext_ping_args {
  static $_TSPEC;


  public function __construct() {
    if (!isset(self::$_TSPEC)) {
      self::$_TSPEC = array(
        );
    }
  }

  public function getName() {
    return 'GetWatchNext_ping_args';
  }

  public function read($input)
  {
    $xfer = 0;
    $fname = null;
    $ftype = 0;
    $fid = 0;
    $xfer += $input->readStructBegin($fname);
    while (true)
    {
      $xfer += $input->readFieldBegin($fname, $ftype, $fid);
      if ($ftype == TType::STOP) {
        break;
      }
      switch ($fid)
      {
        default:
          $xfer += $input->skip($ftype);
          break;
      }
      $xfer += $input->readFieldEnd();
    }
    $xfer += $input->readStructEnd();
    return $xfer;
  }

  public function write($output) {
    $xfer = 0;
    $xfer += $output->writeStructBegin('GetWatchNext_ping_args');
    $xfer += $output->writeFieldStop();
    $xfer += $output->writeStructEnd();
    return $xfer;
  }

}

class GetWatchNext_ping_result {
  static $_TSPEC;


  public function __construct() {
    if (!isset(self::$_TSPEC)) {
      self::$_TSPEC = array(
        );
    }
  }

  public function getName() {
    return 'GetWatchNext_ping_result';
  }

  public function read($input)
  {
    $xfer = 0;
    $fname = null;
    $ftype = 0;
    $fid = 0;
    $xfer += $input->readStructBegin($fname);
    while (true)
    {
      $xfer += $input->readFieldBegin($fname, $ftype, $fid);
      if ($ftype == TType::STOP) {
        break;
      }
      switch ($fid)
      {
        default:
          $xfer += $input->skip($ftype);
          break;
      }
      $xfer += $input->readFieldEnd();
    }
    $xfer += $input->readStructEnd();
    return $xfer;
  }

  public function write($output) {
    $xfer = 0;
    $xfer += $output->writeStructBegin('GetWatchNext_ping_result');
    $xfer += $output->writeFieldStop();
    $xfer += $output->writeStructEnd();
    return $xfer;
  }

}

class GetWatchNext_get_watch_next_args {
  static $_TSPEC;

  /**
   * @var string
   */
  public $req_id = null;
  /**
   * @var string
   */
  public $user_id = null;
  /**
   * @var int
   */
  public $server_no = null;

  public function __construct($vals=null) {
    if (!isset(self::$_TSPEC)) {
      self::$_TSPEC = array(
        1 => array(
          'var' => 'req_id',
          'type' => TType::STRING,
          ),
        2 => array(
          'var' => 'user_id',
          'type' => TType::STRING,
          ),
        3 => array(
          'var' => 'server_no',
          'type' => TType::I32,
          ),
        );
    }
    if (is_array($vals)) {
      if (isset($vals['req_id'])) {
        $this->req_id = $vals['req_id'];
      }
      if (isset($vals['user_id'])) {
        $this->user_id = $vals['user_id'];
      }
      if (isset($vals['server_no'])) {
        $this->server_no = $vals['server_no'];
      }
    }
  }

  public function getName() {
    return 'GetWatchNext_get_watch_next_args';
  }

  public function read($input)
  {
    $xfer = 0;
    $fname = null;
    $ftype = 0;
    $fid = 0;
    $xfer += $input->readStructBegin($fname);
    while (true)
    {
      $xfer += $input->readFieldBegin($fname, $ftype, $fid);
      if ($ftype == TType::STOP) {
        break;
      }
      switch ($fid)
      {
        case 1:
          if ($ftype == TType::STRING) {
            $xfer += $input->readString($this->req_id);
          } else {
            $xfer += $input->skip($ftype);
          }
          break;
        case 2:
          if ($ftype == TType::STRING) {
            $xfer += $input->readString($this->user_id);
          } else {
            $xfer += $input->skip($ftype);
          }
          break;
        case 3:
          if ($ftype == TType::I32) {
            $xfer += $input->readI32($this->server_no);
          } else {
            $xfer += $input->skip($ftype);
          }
          break;
        default:
          $xfer += $input->skip($ftype);
          break;
      }
      $xfer += $input->readFieldEnd();
    }
    $xfer += $input->readStructEnd();
    return $xfer;
  }

  public function write($output) {
    $xfer = 0;
    $xfer += $output->writeStructBegin('GetWatchNext_get_watch_next_args');
    if ($this->req_id !== null) {
      $xfer += $output->writeFieldBegin('req_id', TType::STRING, 1);
      $xfer += $output->writeString($this->req_id);
      $xfer += $output->writeFieldEnd();
    }
    if ($this->user_id !== null) {
      $xfer += $output->writeFieldBegin('user_id', TType::STRING, 2);
      $xfer += $output->writeString($this->user_id);
      $xfer += $output->writeFieldEnd();
    }
    if ($this->server_no !== null) {
      $xfer += $output->writeFieldBegin('server_no', TType::I32, 3);
      $xfer += $output->writeI32($this->server_no);
      $xfer += $output->writeFieldEnd();
    }
    $xfer += $output->writeFieldStop();
    $xfer += $output->writeStructEnd();
    return $xfer;
  }

}


