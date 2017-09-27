<?php
namespace NetflixMicroservices;

$start_time_array = gettimeofday();
$start_time = $start_time_array['sec'] * 1000000 + $start_time_array['usec'];

error_reporting(E_ALL);

$THRIFT_ROOT = './thrift_lib/';
$GEN_DIR = './gen-php/';
require_once $THRIFT_ROOT.'Thrift/ClassLoader/ThriftClassLoader.php';


use NetflixMicroservices\ProcessMovieIDClient;
use NetflixMicroservices\ProcessTextClient;
use NetflixMicroservices\AssignRatingClient;
use NetflixMicroservices\ProcessUniqueIDClient;


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

$IP_ADDR = '128.253.128.65';
$MOVIE_ID_PORT = 9094;
$TEXT_PORT = 9092;
$RATING_PORT = 9093;
$UNIQUE_ID_PORT = 9091;

try {   
    $movie_id_socket = new TSocket($IP_ADDR, $MOVIE_ID_PORT);
    $movie_id_transport = new TBufferedTransport($movie_id_socket, 1024, 1024);
    $movie_id_protocol = new TBinaryProtocol($movie_id_transport);    
    $movie_id_client = new ProcessMovieIDClient($movie_id_protocol);

    $text_socket = new TSocket($IP_ADDR, $TEXT_PORT);
    $text_transport = new TBufferedTransport($text_socket, 1024, 1024);
    $text_protocol = new TBinaryProtocol($text_transport);
    $text_client = new ProcessTextClient($text_protocol);

    $rating_socket = new TSocket($IP_ADDR, $RATING_PORT);
    $rating_transport = new TBufferedTransport($rating_socket, 1024, 1024);
    $rating_protocol = new TBinaryProtocol($rating_transport);
    $rating_client = new AssignRatingClient($rating_protocol);

    $unique_id_socket = new TSocket($IP_ADDR, $UNIQUE_ID_PORT);
    $unique_id_transport = new TBufferedTransport($unique_id_socket, 1024, 1024);
    $unique_id_protocol = new TBinaryProtocol($unique_id_transport);
    $unique_id_client = new ProcessUniqueIDClient($unique_id_protocol);

    
    $req_id = $_POST['user_id']." ".$_SERVER['X-Request-ID'];
    $user_id = $_POST['user_id'];

    $movie_id_transport->open();
    $movie_id = $movie_id_client->process_movie_id($req_id, $user_id, $_POST['url']);
    $movie_id_transport->close();

    $text_transport->open();
    $text_client->process_text($req_id, $user_id, $_POST['text']);
    $text_transport->close();

   
    $unique_id_transport->open();
    $unique_id_client->get_unique_id($req_id, $user_id);
    $unique_id_transport->close();

    $rating_transport->open();
    $rating_client->assign_rating($req_id, $user_id,  $_POST['rating']);
    $rating_transport->close();

    $end_time_array = gettimeofday();
    $end_time = $end_time_array['sec'] * 1000000 + $end_time_array['usec'];
    echo $req_id."\n".(string) $start_time."\n".(string) $end_time;

} catch (TException $tx) {
    print 'TException: '.$tx->getMessage()."\n";
}

?>

    
