<?php

namespace NetflixMicroservices;

error_reporting(E_ALL);

$THRIFT_ROOT = './thrift_lib/';
$GEN_DIR = './gen-php/';
require_once $THRIFT_ROOT.'Thrift/ClassLoader/ThriftClassLoader.php';


use NetflixMicroservices\UserAccountClient;



use Thrift\ClassLoader\ThriftClassLoader;

$loader = new ThriftClassLoader();
$loader->registerNamespace('Thrift', $THRIFT_ROOT);
$loader->registerDefinition('NetflixMicroservices', $GEN_DIR);
$loader->register();

use Thrift\Protocol\TBinaryProtocol;
use Thrift\Transport\TSocket;
use Thrift\Transport\THttpClient;
use Thrift\Transport\TBufferedTransport;
use Thrift\Exception\TException;

$IP_ADDR = '128.253.128.68';
$USER_ACCOUNT_PORT = 10060;

$url = $_SERVER['REQUEST_URI'];
$req_id = $_SERVER['X-Request-ID'];
$content = explode("/", $url);
$user_id = $content[2];
$movie_id = explode(".", $content[3])[0];


try {
    $user_account_socket = new TSocket($IP_ADDR, $USER_ACCOUNT_PORT);
    $user_account_transport = new TBufferedTransport($user_account_socket, 1024, 1024);
    $user_account_protocol = new TBinaryProtocol($user_account_transport);
    $user_account_client = new UserAccountClient($user_account_protocol);

    $user_account_transport->open();
//    $if_success = $user_account_client->if_purchased($_GET["req_id"], $_GET["user_id"], $_GET["movie_id"]);

    $if_success = $user_account_client->if_purchased($req_id, $user_id, $movie_id );
    $user_account_transport->close();



    if ($if_success) {
        return true;
    }

//    if ($movie_id == "movie_4") {
//        header('HTTP/1.0 401 Unauthorized');
//    }

    header('HTTP/1.0 401 Unauthorized');


} catch (TException $tx) {
     print 'TException: '.$tx->getMessage()."\n";
}





?>