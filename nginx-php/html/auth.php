<?php

    namespace NetflixMicroservices;
    $THRIFT_ROOT = './thrift_lib/';
    $GEN_DIR = './gen-php';
    require_once $THRIFT_ROOT.'Thrift/ClassLoader/ThriftClassLoader.php';

    use NetflixMicroservices\UserAccountClient;

    $url = $_SERVER['REQUEST_URI'];
    $content = explode('/', $url);
    $user = $content[2]
    $movie = $content[3]
    if ($content[1] == 'hls') {
        return true;
    }
    //header('HTTP/1.0 401 Unauthorized');
?>