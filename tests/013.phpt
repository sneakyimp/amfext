--TEST--
Decode integer 16384
--SKIPIF--
<?php if (!extension_loaded("amf")) print "skip"; ?>
--FILE--
<?php
$dir = dirname(__FILE__);
$str = file_get_contents($dir . DIRECTORY_SEPARATOR . "amf_int_16384.dat");
var_dump(amf_decode($str));
?>
--EXPECT--
int(16384)

