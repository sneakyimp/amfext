--TEST--
Decode array a=>a
--SKIPIF--
<?php if (!extension_loaded("amf")) print "skip"; ?>
--FILE--
<?php
$dir = dirname(__FILE__);
$str = file_get_contents($dir . DIRECTORY_SEPARATOR . "amf_arr_a_a.dat");
var_dump(amf_decode($str));
?>
--EXPECT--
array(1) {
  ["a"]=>
  string(1) "a"

