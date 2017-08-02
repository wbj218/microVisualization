<?php
namespace TwitterMicroservices;

$THRIFT_ROOT = './thrift_lib/';
$GEN_DIR = './gen-php';
require_once $THRIFT_ROOT.'Thrift/ClassLoader/ThriftClassLoader.php';

use TwitterMicroservices\ReadTimelineClient;
use TwitterMicroservices\ReadTweetClient;

use Thrift\ClassLoader\ThriftClassLoader;
use TwitterMicroservices\BlockStatsClient;

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

    $block_stats_socket = new TSocket('192.168.99.1', 9111);
    $block_stats_transport = new TBufferedTransport($block_stats_socket, 1024, 1024);
    $block_stats_protocol = new TBinaryProtocol($block_stats_transport);    
    $block_stats_client = new BlockStatsClient($block_stats_protocol);

    $block_stats_transport->open();
    $if_blocked = $block_stats_client->if_blocked($_POST["author_id"], $_POST["user_id"]) || 
        $block_stats_client->if_blocked($_POST["author_id"], $_POST["user_id"]);
    $block_stats_transport->close();


    if (!$if_blocked) {
        $rd_timeline_socket = new TSocket('192.168.99.1', 9108);
        $rd_timeline_transport = new TBufferedTransport($rd_timeline_socket, 1024, 1024);
        $rd_timeline_protocol = new TBinaryProtocol($rd_timeline_transport);    
        $rd_timeline_client = new ReadtimelineClient($rd_timeline_protocol);

        $rd_tweet_socket = new TSocket('192.168.99.1', 9107);
        $rd_tweet_transport = new TBufferedTransport($rd_tweet_socket, 1024, 1024);
        $rd_tweet_protocol = new TBinaryProtocol($rd_tweet_transport);    
        $rd_tweet_client = new ReadTweetClient($rd_tweet_protocol);


        $rd_timeline_transport->open();
        $tweet_ids = $rd_timeline_client->read_timeline($_GET["user_id"], $_GET["start_no"], $_GET["n_tweets"], $_GET["type"]);
        $rd_timeline_transport->close();

        foreach ($tweet_ids as $tweet_id) {
            
            $rd_tweet_transport->open();
            $tweet = $rd_tweet_client->read_tweet($_GET["user_id"], $tweet_id, "TWEET");
            $rd_tweet_transport->close();
            echo $tweet->unique_id;
            echo $tweet->text;

        }
    }

} catch (TException $tx) {
    print 'TException: '.$tx->getMessage()."\n";
}

?>
