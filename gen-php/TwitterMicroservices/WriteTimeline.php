<?php
namespace TwitterMicroservices;
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


interface WriteTimelineIf {
  /**
   */
  public function ping();
  /**
   * @param string $user_id
   * @param string $unique_id
   * @param string $req_id
   * @param string $type
   */
  public function write_timeline($user_id, $unique_id, $req_id, $type);
}


class WriteTimelineClient implements \TwitterMicroservices\WriteTimelineIf {
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
    $args = new \TwitterMicroservices\WriteTimeline_ping_args();
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
    if ($bin_accel) $result = thrift_protocol_read_binary($this->input_, '\TwitterMicroservices\WriteTimeline_ping_result', $this->input_->isStrictRead());
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
      $result = new \TwitterMicroservices\WriteTimeline_ping_result();
      $result->read($this->input_);
      $this->input_->readMessageEnd();
    }
    return;
  }

  public function write_timeline($user_id, $unique_id, $req_id, $type)
  {
    $this->send_write_timeline($user_id, $unique_id, $req_id, $type);
  }

  public function send_write_timeline($user_id, $unique_id, $req_id, $type)
  {
    $args = new \TwitterMicroservices\WriteTimeline_write_timeline_args();
    $args->user_id = $user_id;
    $args->unique_id = $unique_id;
    $args->req_id = $req_id;
    $args->type = $type;
    $bin_accel = ($this->output_ instanceof TBinaryProtocolAccelerated) && function_exists('thrift_protocol_write_binary');
    if ($bin_accel)
    {
      thrift_protocol_write_binary($this->output_, 'write_timeline', TMessageType::ONEWAY, $args, $this->seqid_, $this->output_->isStrictWrite());
    }
    else
    {
      $this->output_->writeMessageBegin('write_timeline', TMessageType::ONEWAY, $this->seqid_);
      $args->write($this->output_);
      $this->output_->writeMessageEnd();
      $this->output_->getTransport()->flush();
    }
  }
}


// HELPER FUNCTIONS AND STRUCTURES

class WriteTimeline_ping_args {
  static $_TSPEC;


  public function __construct() {
    if (!isset(self::$_TSPEC)) {
      self::$_TSPEC = array(
        );
    }
  }

  public function getName() {
    return 'WriteTimeline_ping_args';
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
    $xfer += $output->writeStructBegin('WriteTimeline_ping_args');
    $xfer += $output->writeFieldStop();
    $xfer += $output->writeStructEnd();
    return $xfer;
  }

}

class WriteTimeline_ping_result {
  static $_TSPEC;


  public function __construct() {
    if (!isset(self::$_TSPEC)) {
      self::$_TSPEC = array(
        );
    }
  }

  public function getName() {
    return 'WriteTimeline_ping_result';
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
    $xfer += $output->writeStructBegin('WriteTimeline_ping_result');
    $xfer += $output->writeFieldStop();
    $xfer += $output->writeStructEnd();
    return $xfer;
  }

}

class WriteTimeline_write_timeline_args {
  static $_TSPEC;

  /**
   * @var string
   */
  public $user_id = null;
  /**
   * @var string
   */
  public $unique_id = null;
  /**
   * @var string
   */
  public $req_id = null;
  /**
   * @var string
   */
  public $type = null;

  public function __construct($vals=null) {
    if (!isset(self::$_TSPEC)) {
      self::$_TSPEC = array(
        1 => array(
          'var' => 'user_id',
          'type' => TType::STRING,
          ),
        2 => array(
          'var' => 'unique_id',
          'type' => TType::STRING,
          ),
        3 => array(
          'var' => 'req_id',
          'type' => TType::STRING,
          ),
        4 => array(
          'var' => 'type',
          'type' => TType::STRING,
          ),
        );
    }
    if (is_array($vals)) {
      if (isset($vals['user_id'])) {
        $this->user_id = $vals['user_id'];
      }
      if (isset($vals['unique_id'])) {
        $this->unique_id = $vals['unique_id'];
      }
      if (isset($vals['req_id'])) {
        $this->req_id = $vals['req_id'];
      }
      if (isset($vals['type'])) {
        $this->type = $vals['type'];
      }
    }
  }

  public function getName() {
    return 'WriteTimeline_write_timeline_args';
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
            $xfer += $input->readString($this->user_id);
          } else {
            $xfer += $input->skip($ftype);
          }
          break;
        case 2:
          if ($ftype == TType::STRING) {
            $xfer += $input->readString($this->unique_id);
          } else {
            $xfer += $input->skip($ftype);
          }
          break;
        case 3:
          if ($ftype == TType::STRING) {
            $xfer += $input->readString($this->req_id);
          } else {
            $xfer += $input->skip($ftype);
          }
          break;
        case 4:
          if ($ftype == TType::STRING) {
            $xfer += $input->readString($this->type);
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
    $xfer += $output->writeStructBegin('WriteTimeline_write_timeline_args');
    if ($this->user_id !== null) {
      $xfer += $output->writeFieldBegin('user_id', TType::STRING, 1);
      $xfer += $output->writeString($this->user_id);
      $xfer += $output->writeFieldEnd();
    }
    if ($this->unique_id !== null) {
      $xfer += $output->writeFieldBegin('unique_id', TType::STRING, 2);
      $xfer += $output->writeString($this->unique_id);
      $xfer += $output->writeFieldEnd();
    }
    if ($this->req_id !== null) {
      $xfer += $output->writeFieldBegin('req_id', TType::STRING, 3);
      $xfer += $output->writeString($this->req_id);
      $xfer += $output->writeFieldEnd();
    }
    if ($this->type !== null) {
      $xfer += $output->writeFieldBegin('type', TType::STRING, 4);
      $xfer += $output->writeString($this->type);
      $xfer += $output->writeFieldEnd();
    }
    $xfer += $output->writeFieldStop();
    $xfer += $output->writeStructEnd();
    return $xfer;
  }

}


