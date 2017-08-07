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


interface UserReviewDBIf {
  /**
   */
  public function ping();
  /**
   * @param string $req_id
   * @param string $movie_id
   * @param string $user_id
   * @param string $unique_id
   */
  public function write_user_review($req_id, $movie_id, $user_id, $unique_id);
}


class UserReviewDBClient implements \NetflixMicroservices\UserReviewDBIf {
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
    $args = new \NetflixMicroservices\UserReviewDB_ping_args();
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
    if ($bin_accel) $result = thrift_protocol_read_binary($this->input_, '\NetflixMicroservices\UserReviewDB_ping_result', $this->input_->isStrictRead());
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
      $result = new \NetflixMicroservices\UserReviewDB_ping_result();
      $result->read($this->input_);
      $this->input_->readMessageEnd();
    }
    return;
  }

  public function write_user_review($req_id, $movie_id, $user_id, $unique_id)
  {
    $this->send_write_user_review($req_id, $movie_id, $user_id, $unique_id);
  }

  public function send_write_user_review($req_id, $movie_id, $user_id, $unique_id)
  {
    $args = new \NetflixMicroservices\UserReviewDB_write_user_review_args();
    $args->req_id = $req_id;
    $args->movie_id = $movie_id;
    $args->user_id = $user_id;
    $args->unique_id = $unique_id;
    $bin_accel = ($this->output_ instanceof TBinaryProtocolAccelerated) && function_exists('thrift_protocol_write_binary');
    if ($bin_accel)
    {
      thrift_protocol_write_binary($this->output_, 'write_user_review', TMessageType::ONEWAY, $args, $this->seqid_, $this->output_->isStrictWrite());
    }
    else
    {
      $this->output_->writeMessageBegin('write_user_review', TMessageType::ONEWAY, $this->seqid_);
      $args->write($this->output_);
      $this->output_->writeMessageEnd();
      $this->output_->getTransport()->flush();
    }
  }
}


// HELPER FUNCTIONS AND STRUCTURES

class UserReviewDB_ping_args {
  static $_TSPEC;


  public function __construct() {
    if (!isset(self::$_TSPEC)) {
      self::$_TSPEC = array(
        );
    }
  }

  public function getName() {
    return 'UserReviewDB_ping_args';
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
    $xfer += $output->writeStructBegin('UserReviewDB_ping_args');
    $xfer += $output->writeFieldStop();
    $xfer += $output->writeStructEnd();
    return $xfer;
  }

}

class UserReviewDB_ping_result {
  static $_TSPEC;


  public function __construct() {
    if (!isset(self::$_TSPEC)) {
      self::$_TSPEC = array(
        );
    }
  }

  public function getName() {
    return 'UserReviewDB_ping_result';
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
    $xfer += $output->writeStructBegin('UserReviewDB_ping_result');
    $xfer += $output->writeFieldStop();
    $xfer += $output->writeStructEnd();
    return $xfer;
  }

}

class UserReviewDB_write_user_review_args {
  static $_TSPEC;

  /**
   * @var string
   */
  public $req_id = null;
  /**
   * @var string
   */
  public $movie_id = null;
  /**
   * @var string
   */
  public $user_id = null;
  /**
   * @var string
   */
  public $unique_id = null;

  public function __construct($vals=null) {
    if (!isset(self::$_TSPEC)) {
      self::$_TSPEC = array(
        1 => array(
          'var' => 'req_id',
          'type' => TType::STRING,
          ),
        2 => array(
          'var' => 'movie_id',
          'type' => TType::STRING,
          ),
        3 => array(
          'var' => 'user_id',
          'type' => TType::STRING,
          ),
        4 => array(
          'var' => 'unique_id',
          'type' => TType::STRING,
          ),
        );
    }
    if (is_array($vals)) {
      if (isset($vals['req_id'])) {
        $this->req_id = $vals['req_id'];
      }
      if (isset($vals['movie_id'])) {
        $this->movie_id = $vals['movie_id'];
      }
      if (isset($vals['user_id'])) {
        $this->user_id = $vals['user_id'];
      }
      if (isset($vals['unique_id'])) {
        $this->unique_id = $vals['unique_id'];
      }
    }
  }

  public function getName() {
    return 'UserReviewDB_write_user_review_args';
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
            $xfer += $input->readString($this->movie_id);
          } else {
            $xfer += $input->skip($ftype);
          }
          break;
        case 3:
          if ($ftype == TType::STRING) {
            $xfer += $input->readString($this->user_id);
          } else {
            $xfer += $input->skip($ftype);
          }
          break;
        case 4:
          if ($ftype == TType::STRING) {
            $xfer += $input->readString($this->unique_id);
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
    $xfer += $output->writeStructBegin('UserReviewDB_write_user_review_args');
    if ($this->req_id !== null) {
      $xfer += $output->writeFieldBegin('req_id', TType::STRING, 1);
      $xfer += $output->writeString($this->req_id);
      $xfer += $output->writeFieldEnd();
    }
    if ($this->movie_id !== null) {
      $xfer += $output->writeFieldBegin('movie_id', TType::STRING, 2);
      $xfer += $output->writeString($this->movie_id);
      $xfer += $output->writeFieldEnd();
    }
    if ($this->user_id !== null) {
      $xfer += $output->writeFieldBegin('user_id', TType::STRING, 3);
      $xfer += $output->writeString($this->user_id);
      $xfer += $output->writeFieldEnd();
    }
    if ($this->unique_id !== null) {
      $xfer += $output->writeFieldBegin('unique_id', TType::STRING, 4);
      $xfer += $output->writeString($this->unique_id);
      $xfer += $output->writeFieldEnd();
    }
    $xfer += $output->writeFieldStop();
    $xfer += $output->writeStructEnd();
    return $xfer;
  }

}


