--TEST--
Check for amf presence
--SKIPIF--
<?php if (!extension_loaded("amf")) print "skip"; ?>
--FILE--
<?php
echo "amf extension is available";
?>
--EXPECT--
amf extension is available
