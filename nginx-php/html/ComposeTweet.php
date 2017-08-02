<?php

namespace TwitterMicroservices;

$THRIFT_ROOT = './thrift_lib/';
$GEN_DIR = './gen-php';
require_once $THRIFT_ROOT.'Thrift/ClassLoader/ThriftClassLoader.php';


use TwitterMicroservices\UniqueIDClient;
use TwitterMicroservices\ProcessImageClient;
use TwitterMicroservices\ProcessTextClient;
use TwitterMicroservices\ProcessVideoClient;
use TwitterMicroservices\ProcessURLClient;
use TwitterMicroservices\ProcessTagClient;
use TwitterMicroservices\ComposeTweetClient;

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

$IP_ADDR = "128.253.128.65";

try {   
    $uniqueid_socket = new TSocket($IP_ADDR, 9096);
    $uniqueid_transport = new TBufferedTransport($uniqueid_socket, 1024, 1024);
    $uniqueid_protocol = new TBinaryProtocol($uniqueid_transport);    
    $uniqueid_client = new ProcessUniqueIDClient($uniqueid_protocol);

    $text_socket = new TSocket($IP_ADDR, 9091);
    $text_transport = new TBufferedTransport($text_socket, 1024, 1024);
    $text_protocol = new TBinaryProtocol($text_transport);
    $text_client = new ProcessTextClient($text_protocol);

    $image_socket = new TSocket($IP_ADDR, 9092);
    $image_transport = new TBufferedTransport($image_socket, 1024, 1024);
    $image_protocol = new TBinaryProtocol($image_transport);
    $image_client = new ProcessImageClient($image_protocol);

    $video_socket = new TSocket($IP_ADDR, 9093);
    $video_transport = new TBufferedTransport($video_socket, 1024, 1024);
    $video_protocol = new TBinaryProtocol($video_transport);
    $video_client = new ProcessVideoClient($video_protocol);

    $urlshortener_socket = new TSocket($IP_ADDR, 9094);
    $urlshortener_transport = new TBufferedTransport($urlshortener_socket, 1024, 1024);
    $urlshortener_protocol = new TBinaryProtocol($urlshortener_transport);
    $urlshortener_client = new ProcessURLClient($urlshortener_protocol);

    $tag_socket = new TSocket($IP_ADDR, 9095);
    $tag_transport = new TBufferedTransport($tag_socket, 1024, 1024);
    $tag_protocol = new TBinaryProtocol($tag_transport);
    $tag_client = new ProcessTagClient($tag_protocol);

    $tweet_socket = new TSocket($IP_ADDR, 9090);
    $tweet_transport = new TBufferedTransport($tweet_socket, 1024, 1024);
    $tweet_protocol = new TBinaryProtocol($tweet_transport);
    $tweet_client = new ComposeTweetClient($tweet_protocol);

    $tmp_id = $_POST['tmp_id'];
    echo $tmp_id;
    
    

    $uniqueid_transport->open();        
    $uniqueid_client->get_unique_id($tmp_id);    
    $uniqueid_transport->close();

    if ($_POST["text"]) {

        $urlshortener_transport->open();        
        $urlshortener_client->process_url($tmp_id, $_POST['text']);    
        $urlshortener_transport->close();
        
        $tag_transport->open();
        $tag_client->process_tag($tmp_id, $_POST['text']); 
        $tag_transport->close();
        
        $text_transport->open();        
        $text_client->process_text($tmp_id, $_POST['text']); 
        
        $text_transport->close();
    }

    
    if ($_POST["image"]) {
        $image_transport->open(); 
        
        $image_client->process_image($tmp_id, $_POST['image']);    
        $image_transport->close();
    }
    else{
        $image_transport->open();       
        $image_client->process_image($tmp_id, "");    
        $image_transport->close();
    }

    if ($_POST["video"]) {
        $video_transport->open();  
        // echo $_POST["video"];        
        $video_client->process_video($tmp_id, $_POST['video']);    
        $video_transport->close();
    }
    else{
        $video_transport->open();       
        $video_client->process_video($tmp_id, "");    
        $video_transport->close();
    }

}   catch (TException $tx) {
    print 'TException: '.$tx->getMessage()."\n";
}
?>