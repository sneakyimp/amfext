--TEST--
Array encode/decode
--SKIPIF--
<?php if (!extension_loaded("amf")) print "skip"; ?>
--FILE--
<?php
include 'helpers.php';

encodeAndDecode([]);
encodeAndDecode([true]);
encodeAndDecode([1, true, 'string', null]);
encodeAndDecode(['a' => 1, 'b' => 3]);
encodeAndDecode(['a' => 1, 5, 6]);

?>
--EXPECT--
string(6) "090101"
array(0) {
}
OK

string(8) "09030103"
array(1) {
  [0]=>
  bool(true)
}
OK

string(30) "090901040103060d737472696e6701"
array(4) {
  [0]=>
  int(1)
  [1]=>
  bool(true)
  [2]=>
  string(6) "string"
  [3]=>
  NULL
}
OK

string(22) "0901036104010362040301"
array(2) {
  ["a"]=>
  int(1)
  ["b"]=>
  int(3)
}
OK

string(22) "0905036104010104050406"
array(3) {
  ["a"]=>
  int(1)
  [0]=>
  int(5)
  [1]=>
  int(6)
}
OK

