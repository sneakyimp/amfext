--TEST--
Decode integer -1
--SKIPIF--
<?php if (!extension_loaded("amf")) print "skip"; ?>
--FILE--
<?php
$dir = dirname(__FILE__);
$str = file_get_contents($dir . DIRECTORY_SEPARATOR . "amf_int_-1.dat");
var_dump(amf_decode($str));
?>
--EXPECT--
int(-1)

