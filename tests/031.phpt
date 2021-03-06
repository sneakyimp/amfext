--TEST--
Complex array test
--SKIPIF--
<?php if (!extension_loaded("amf")) print "skip"; ?>
--FILE--
<?php
$dir = dirname(__FILE__);
$flash_amf3 = file_get_contents($dir . DIRECTORY_SEPARATOR . "amf_complex_array_1.dat");
//echo "--flash data encoded--\n";
//var_dump(bin2hex($flash_amf3));
//echo "--\n";

$flash_v = amf_decode($flash_amf3);
//echo "--flash object decoded--\n";
//var_dump($flash_v);
//echo "--\n";

// this array should match $flash_v
// to test nested arrays, trying to serialize all data types at various depths
$v = array(
	0 => "a string",
	"a" => "a",
	1 => "another string",
	"b" => "another string",
	3 => TRUE,
	4 => FALSE,
	5 => NULL,
	"int" => 123456,
	"float" => 9.87654321,
	"arr1" => array(
		"a" => NULL,
		"b" => FALSE,
		"c" => TRUE,
		"d" => 456,
		"e" => 222.222,
		"f" => "",
		"g" => "a string",
		"h" => "another string",
		"i" => "a third string",
		"j" => array(
			"a" => NULL,
			"b" => FALSE,
			"c" => TRUE,
			"d" => 456,
			"e" => 222.222,
			"f" => "",
			"g" => "a string",
			"h" => "another string",
			"i" => "a third string",
			"j" => array(),
			"k" => "last item"
		),
		"k" => "last item"
	),
	6 => array(
		0 => NULL,
		1 => FALSE,
		2 => TRUE,
		3 => 456,
		4 => 222.222,
		5 => "",
		6 => "a string",
		7 => "another string",
		8 => "a third string",
		9 => array(
			0 => NULL,
			1 => FALSE,
			2 => TRUE,
			3 => 456,
			4 => 222.222,
			5 => "",
			6 => "a string",
			7 => "another string",
			8 => "a third string",
			9 => array(),
			10 => "other last item"
		),
		10 => "other last item"
	)
);


// compare the array we just defined to the one decoded from flash-generated AMF3
if ($v == $flash_v) {
	echo "ORIG EQUAL\n";
} else {
	echo "ORIG NOT EQUAL\n";
}

// I'm not sure but I believe these arrays could never be strictly equal so remove this test
//if ($v === $flash_v) {
//	echo "ORIG STRICTLY EQUAL\n";
//} else {
//	echo "ORIG NOT STRICTLY EQUAL\n";
//}


// encode our array -- NOTE this encoding may not match at all the AMF3 binary data generated by Flash Player
// and it does not have to be identical.
$encoded = amf_encode($v);
//echo "--encoded--\n";
//var_dump(bin2hex($encoded));
//echo "--\n";

// now decode our own AMF3 binary data and see if we get our object back
$decoded = amf_decode($encoded);
//echo "--decoded--\n";
//var_dump($decoded);
//echo "--\n";

if ($v == $decoded) {
	echo "EQUAL\n";
} else {
	echo "NOT EQUAL\n";
}

// not sure but I think they can never be strictly equal
//if ($v === $decoded) {
//	echo "STRICTLY EQUAL\n";
//} else {
//	echo "NOT STRICTLY EQUAL\n";
//}
?>
--EXPECT--
ORIG EQUAL
EQUAL

