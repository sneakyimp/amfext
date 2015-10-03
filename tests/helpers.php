<?php

function encodeAndDecode($input) {
  $encoded = amf_encode($input);
  var_dump(bin2hex($encoded));

  $decoded = amf_decode($encoded);
  if ($input === $decoded) {
    var_dump($input);
    echo "OK\n\n";
  } else {
    var_dump($input);
    echo "Encode/decode created\n";
    var_dump($decoded);
    echo "FAILED\n\n";
  }
}
