--TEST--
Number encode/decode
--SKIPIF--
<?php if (!extension_loaded("amf")) print "skip"; ?>
--FILE--
<?php
include 'helpers.php';

encodeAndDecode(0);
encodeAndDecode(10);
encodeAndDecode(5000000000);

?>
--EXPECT--
string(4) "0400"
int(0)
OK

string(4) "040a"
int(10)
OK

TODO test failure Sean-Der
