--TEST--
Decode double 3.14159
--SKIPIF--
<?php if (!extension_loaded("amf")) print "skip"; ?>
--FILE--
<?php
$dir = dirname(__FILE__);
$str = file_get_contents($dir . DIRECTORY_SEPARATOR . "amf_double_3.14159.dat");
var_dump(amf_decode($str));
?>
--EXPECT--
float(3.14159)


