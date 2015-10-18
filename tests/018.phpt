--TEST--
Decode integer 268435456
--SKIPIF--
<?php if (!extension_loaded("amf")) print "skip"; ?>
--FILE--
<?php
$dir = dirname(__FILE__);
// this value is actually one higher than the highest int serializable by AMF3 so it is serialized as an 8-byte double
$str = file_get_contents($dir . DIRECTORY_SEPARATOR . "amf_int_268435456.dat");
var_dump(amf_decode($str));
?>
--EXPECT--
float(268435456)

