<?php
namespace TwitterMicroservices;

$THRIFT_ROOT = './thrift_lib/';
$GEN_DIR = './gen-php';
require_once $THRIFT_ROOT.'Thrift/ClassLoader/ThriftClassLoader.php';

use TwitterMicroservices\WriteFavoriteClient;


use Thrift\ClassLoader\ThriftClassLoader;

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
    $wt_favorite_socket = new TSocket('192.168.99.1', 9110);
    $wt_favorite_transport = new TBufferedTransport($wt_favorite_socket, 1024, 1024);
    $wt_favorite_protocol = new TBinaryProtocol($wt_favorite_transport);    
    $wt_favorite_client = new WriteFavoriteClient($wt_favorite_protocol);

    $wt_favorite_transport->open();
    $wt_favorite_client->write_favorite($_POST["user_id"], $_POST["tweet_id"], $_POST["author_id"]);
    $wt_favorite_transport->close();


} catch (TException $tx) {
    print 'TException: '.$tx->getMessage()."\n";
}


?>

