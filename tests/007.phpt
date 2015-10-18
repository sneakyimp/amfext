--TEST--
Decode FALSE
--SKIPIF--
<?php if (!extension_loaded("amf")) print "skip"; ?>
--FILE--
<?php
$dir = dirname(__FILE__);
$str = file_get_contents($dir . DIRECTORY_SEPARATOR . "amf_bool_false.dat");
var_dump(amf_decode($str));
?>
--EXPECT--
bool(false)
