--TEST--
Decode integer 5000000000
--SKIPIF--
<?php if (!extension_loaded("amf")) print "skip"; ?>
--FILE--
<?php
$dir = dirname(__FILE__);
$str = file_get_contents($dir . DIRECTORY_SEPARATOR . "amf_int_5000000000.dat");
var_dump(amf_decode($str));
?>
--EXPECT--
float(5000000000)

