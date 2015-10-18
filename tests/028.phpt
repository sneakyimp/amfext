--TEST--
Decode string abc
--SKIPIF--
<?php if (!extension_loaded("amf")) print "skip"; ?>
--FILE--
<?php
$dir = dirname(__FILE__);
$str = file_get_contents($dir . DIRECTORY_SEPARATOR . "amf_str_abc.dat");
var_dump(amf_decode($str));
?>
--EXPECT--
string(3) "abc"
