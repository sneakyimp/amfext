--TEST--
String encode/decode
--SKIPIF--
<?php if (!extension_loaded("amf")) print "skip"; ?>
--FILE--
<?php
include 'helpers.php';

encodeAndDecode('');
encodeAndDecode('testing');
encodeAndDecode('testing $123');

?>
--EXPECT--
string(4) "0601"
string(0) ""
OK

string(18) "060f74657374696e67"
string(7) "testing"
OK

string(28) "061974657374696e672024313233"
string(12) "testing $123"
OK

