--TEST--
Decode integer 268435455
--SKIPIF--
<?php if (!extension_loaded("amf")) print "skip"; ?>
--FILE--
<?php
$dir = dirname(__FILE__);
$str = file_get_contents($dir . DIRECTORY_SEPARATOR . "amf_int_268435455.dat");
var_dump(amf_decode($str));
?>
--EXPECT--
int(268435455)
