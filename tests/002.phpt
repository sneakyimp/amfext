--TEST--
Bool+null encode/decode
--SKIPIF--
<?php if (!extension_loaded("amf")) print "skip"; ?>
--FILE--
<?php
include 'helpers.php';

encodeAndDecode(true);
encodeAndDecode(false);
encodeAndDecode(null);

?>
--EXPECT--
string(2) "03"
bool(true)
OK

string(2) "02"
bool(false)
OK

string(2) "01"
NULL
OK

