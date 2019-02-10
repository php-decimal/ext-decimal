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
$number = new \Decimal\Decimal(1);

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
object(Decimal\Decimal)#1 (2) {
  ["value"]=>
  string(35) "0.000004263976243931729575375414851"
  ["precision"]=>
  int(28)
}
