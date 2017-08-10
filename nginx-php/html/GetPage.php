<?php
namespace NetflixMicroservices;

error_reporting(E_ALL);

$THRIFT_ROOT = './thrift_lib/';
$GEN_DIR = './gen-php/';
require_once $THRIFT_ROOT.'Thrift/ClassLoader/ThriftClassLoader.php';




use NetflixMicroservices\ComposePageClient;
use NetflixMicroservices\ProcessMovieIDClient;

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
$NUM_COMPOSE_PAGE_CLIENT = 2;
$MOVIE_ID_PORT = 9094;
$COMPOSE_PAGE_PORT = 10050;



try {   
    $movie_id_socket = new TSocket($IP_ADDR, $MOVIE_ID_PORT);
    $movie_id_transport = new TBufferedTransport($movie_id_socket, 1024, 1024);
    $movie_id_protocol = new TBinaryProtocol($movie_id_transport);    
    $movie_id_client = new ProcessMovieIDClient($movie_id_protocol);

    $port = rand(0, $NUM_COMPOSE_PAGE_CLIENT - 1);
    $compose_page_socket = new TSocket($IP_ADDR, $COMPOSE_PAGE_PORT + $port);
    $compose_page_transport = new TBufferedTransport($compose_page_socket, 1024, 1024);
    $compose_page_protocol = new TBinaryProtocol($compose_page_transport);
    $compose_page_client = new ComposePageClient($compose_page_protocol);

    $req_id = $_GET['req_id'];
    // echo $req_id;

    $movie_id_transport->open();
    $movie_id = $movie_id_client->get_movie_id($req_id, $_GET['url']);
    $movie_id_transport->close();

    $page = new MoviePage();

    $compose_page_transport->open();
    $page = $compose_page_client->compose_page($req_id, $movie_id, $_GET['user_id']);
    $compose_page_transport->close();

    // echo $page->rating;

}   catch (TException $tx) {
        print 'TException: '.$tx->getMessage()."\n";
}
?>



