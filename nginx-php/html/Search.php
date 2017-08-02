<?php

namespace TwitterMicroservices;

$THRIFT_ROOT = './thrift_lib/';
$GEN_DIR = './gen-php';
require_once $THRIFT_ROOT.'Thrift/ClassLoader/ThriftClassLoader.php';


use TwitterMicroservices\XapianClient;


use Thrift\ClassLoader\ThriftClassLoader;

$IP_ADDR = '128.253.128.65';

$loader = new ThriftClassLoader();
$loader->registerNamespace('Thrift', $THRIFT_ROOT);
$loader->registerDefinition('TwitterMicroservice', $GEN_DIR);
$loader->register();

use Thrift\Protocol\TBinaryProtocol;
use Thrift\Transport\TSocket;
use Thrift\Transport\THttpClient;
use Thrift\Transport\TBufferedTransport;
use Thrift\Exception\TException;



try {   
    $xapian_socket = new TSocket($IP_ADDR, 9101);
    $xapian_transport = new TBufferedTransport($xapian_socket, 1024, 1024);
    $xapian_protocol = new TBinaryProtocol($xapian_transport);    
    $xapian_client = new XapianClient($xapian_protocol);

    $xapian_transport->open();
    echo $xapian_client->search($_GET["key"], $_GET["req_id"]);
    $xapian_transport->close();


}catch (TException $tx) {
    print 'TException: '.$tx->getMessage()."\n";
}
?>
