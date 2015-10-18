--TEST--
Decode double -1.5
--SKIPIF--
<?php if (!extension_loaded("amf")) print "skip"; ?>
--FILE--
<?php
$dir = dirname(__FILE__);
$str = file_get_contents($dir . DIRECTORY_SEPARATOR . "amf_double_-1.5.dat");
var_dump(amf_decode($str));
?>
--EXPECT--
float(-1.5)

