--TEST--
Decode empty string
--SKIPIF--
<?php if (!extension_loaded("amf")) print "skip"; ?>
--FILE--
<?php
$dir = dirname(__FILE__);
$str = file_get_contents($dir . DIRECTORY_SEPARATOR . "amf_str_empty.dat");
var_dump(amf_decode($str));
?>
--EXPECT--
string(0) ""
