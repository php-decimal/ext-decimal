--TEST--
Disable OPcache pass 2
--SKIPIF--
<?php
if (!function_exists("opcache_get_status")) echo "skip";
?>
--INI--
opcache.enable=1
opcache.enable_cli=1
opcache.optimization_level=2
--FILE--
<?php

/**
 * Test that opcache does not convert these constant strings to float.
 */
$number = \Decimal\Rational::valueOf(1);

$number = $number *  "0.1";
$number = $number *  "0.0000123456789";
$number = $number *  "12345.6789";
$number = $number * "-98765.4321";
$number = $number *  "9.87654321";
$number = $number * "-1.23456789";
$number = $number *  "0.00000000123456789";
$number = $number * "-0.00000000123456789";
$number = $number *  "12345.6789";
$number = $number * "-12345.6789";

var_dump($number);

?>
--EXPECT--
object(Decimal\Rational)#1 (2) {
  ["num"]=>
  string(75) "426397624393172957537541485031321559583108704527981359123508334688202542589"
  ["den"]=>
  string(5) "1E+80"
}
