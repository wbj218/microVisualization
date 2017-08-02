<?php
namespace TwitterMicroservices;

$THRIFT_ROOT = './thrift_lib/';
$GEN_DIR = './gen-php';
require_once $THRIFT_ROOT.'Thrift/ClassLoader/ThriftClassLoader.php';

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

try {  
    $block_stats_socket = new TSocket('192.168.99.1', 9111);
    $block_stats_transport = new TBufferedTransport($block_stats_socket, 1024, 1024);
    $block_stats_protocol = new TBinaryProtocol($block_stats_transport);    
    $block_stats_client = new BlockStatsClient($block_stats_protocol);

    
    $block_stats_transport->open();
    if ($_POST["op"] == "add_block")
        $block_stats_client->add_block($_POST["user_1"], $_POST["user_2"]);
    elseif ($_POST["op"] == "remove_block")
        $block_stats_client->remove_block($_POST["user_1"], $_POST["user_2"]);
    elseif ($_POST["op"] == "if_blocked")
        echo $block_stats_client->if_blocked($_POST["user_1"], $_POST["user_2"], "11111") ? 'true': 'false';
    else
        echo "invalid op code";
    
    $block_stats_transport->close();

} catch (TException $tx) {
    print 'TException: '.$tx->getMessage()."\n";
}

?>