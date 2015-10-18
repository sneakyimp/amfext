--TEST--
Decode array a=>1, b=>3
--SKIPIF--
<?php if (!extension_loaded("amf")) print "skip"; ?>
--FILE--
<?php
$dir = dirname(__FILE__);
$str = file_get_contents($dir . DIRECTORY_SEPARATOR . "amf_arr_a1b3.dat");
var_dump(amf_decode($str));
?>
--EXPECT--
array(2) {
  ["a"]=>
  int(1)
  ["b"]=>
  int(3)
}



