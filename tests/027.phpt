--TEST--
Decode array abc
--SKIPIF--
<?php if (!extension_loaded("amf")) print "skip"; ?>
--FILE--
<?php
$dir = dirname(__FILE__);
$str = file_get_contents($dir . DIRECTORY_SEPARATOR . "amf_arr_abc.dat");
var_dump(amf_decode($str));
?>
--EXPECT--
array(3) {
  [0]=>
  string(3) "abc"
  [1]=>
  string(3) "abc"
  [2]=>
  string(3) "abc"
}

