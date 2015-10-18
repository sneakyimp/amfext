--TEST--
Decode integer 2097151
--SKIPIF--
<?php if (!extension_loaded("amf")) print "skip"; ?>
--FILE--
<?php
$dir = dirname(__FILE__);
$str = file_get_contents($dir . DIRECTORY_SEPARATOR . "amf_int_2097151.dat");
var_dump(amf_decode($str));
?>
--EXPECT--
int(2097151)

