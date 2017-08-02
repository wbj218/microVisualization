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
use TwitterMicroservices\ComposeRetweetClient;
use TwitterMicroservices\ReadTweetClient;
use TwitterMicroservices\BlockStatsClient;

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

$IP_ADDR = '128.253.128.65';


try {   

    $block_stats_socket = new TSocket($IP_ADDR, 9111);
    $block_stats_transport = new TBufferedTransport($block_stats_socket, 1024, 1024);
    $block_stats_protocol = new TBinaryProtocol($block_stats_transport);    
    $block_stats_client = new BlockStatsClient($block_stats_protocol);

    $block_stats_transport->open();
    $if_blocked = $block_stats_client->if_blocked($_POST["author_id"], $_POST["user_id"], $_POST["req_id"]) || 
        $block_stats_client->if_blocked($_POST["user_id"], $_POST["author_id"], $_POST["req_id"]);
    $block_stats_transport->close();

    if (!$if_blocked) {
        $uniqueid_socket = new TSocket($IP_ADDR, 9096);
        $uniqueid_transport = new TBufferedTransport($uniqueid_socket, 1024, 1024);
        $uniqueid_protocol = new TBinaryProtocol($uniqueid_transport);    
        $uniqueid_client = new ProcessUniqueIDClient($uniqueid_protocol);

        $text_socket = new TSocket($IP_ADDR, 9091);
        $text_transport = new TBufferedTransport($text_socket, 1024, 1024);
        $text_protocol = new TBinaryProtocol($text_transport);
        $text_client = new ProcessTextClient($text_protocol);

        // $image_socket = new TSocket($IP_ADDR, 9092);
        // $image_transport = new TBufferedTransport($image_socket, 1024, 1024);
        // $image_protocol = new TBinaryProtocol($image_transport);
        // $image_client = new ProcessImageClient($image_protocol);

        // $video_socket = new TSocket($IP_ADDR, 9093);
        // $video_transport = new TBufferedTransport($video_socket, 1024, 1024);
        // $video_protocol = new TBinaryProtocol($video_transport);
        // $video_client = new ProcessVideoClient($video_protocol);

        $urlshortener_socket = new TSocket($IP_ADDR, 9094);
        $urlshortener_transport = new TBufferedTransport($urlshortener_socket, 1024, 1024);
        $urlshortener_protocol = new TBinaryProtocol($urlshortener_transport);
        $urlshortener_client = new ProcessURLClient($urlshortener_protocol);

        $tag_socket = new TSocket($IP_ADDR, 9095);
        $tag_transport = new TBufferedTransport($tag_socket, 1024, 1024);
        $tag_protocol = new TBinaryProtocol($tag_transport);
        $tag_client = new ProcessTagClient($tag_protocol);

        $retweet_socket = new TSocket($IP_ADDR, 9104);
        $retweet_transport = new TBufferedTransport($retweet_socket, 1024, 1024);
        $retweet_protocol = new TBinaryProtocol($retweet_transport);
        $retweet_client = new ComposeRetweetClient($retweet_protocol);

        $read_tweet_socket = new TSocket($IP_ADDR, 9107);
        $read_tweet_transport = new TBufferedTransport($read_tweet_socket, 1024, 1024);
        $read_tweet_protocol = new TBinaryProtocol($read_tweet_transport);
        $read_tweet_client = new ReadTweetClient($read_tweet_protocol);


        $req_id = $_POST['req_id'];
        echo $req_id;
       

        $uniqueid_transport->open();        
        $uniqueid_client->get_unique_id($req_id, "RETWEET");    
        $uniqueid_transport->close();

        if ($_POST["text"]) {

            $urlshortener_transport->open();        
            $urlshortener_client->process_url($req_id, $_POST['text'], "RETWEET");    
            $urlshortener_transport->close();
            
            $tag_transport->open();
            $tag_client->process_tag($req_id, $_POST['text'], "RETWEET"); 
            $tag_transport->close();
            
            $text_transport->open();        
            $text_client->process_text($req_id, $_POST['text'], "RETWEET");         
            $text_transport->close();
        }



        // if ($_POST["tweet_id"] && $_POST["author_id"]) {

          
        echo "send read".$_POST["tweet_id"]. $_POST["author_id"];
        $read_tweet_transport->open();
        $tweet = $read_tweet_client->read_tweet($_POST["author_id"], $_POST["tweet_id"], "TWEET", $req_id);
        $read_tweet_transport->close();

        

        $retweet_transport->open();
        $retweet_client->upload_tweet($tweet, $req_id);
        $retweet_transport->close();

        // }   
    }

}   catch (TException $tx) {
    print 'TException: '.$tx->getMessage()."\n";
}
?>
