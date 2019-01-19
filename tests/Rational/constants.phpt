--TEST--
Rational constants
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo 'skip';
?>
--FILE--
<?php
use Decimal\Rational;

var_dump(Rational::DEFAULT_PRECISION);
var_dump(Rational::DEFAULT_ROUNDING);

var_dump(Rational::ROUND_UP);
var_dump(Rational::ROUND_DOWN);
var_dump(Rational::ROUND_CEILING);
var_dump(Rational::ROUND_FLOOR);
var_dump(Rational::ROUND_HALF_UP);
var_dump(Rational::ROUND_HALF_DOWN);
var_dump(Rational::ROUND_HALF_EVEN);
var_dump(Rational::ROUND_HALF_ODD);

?>
--EXPECTF--
int(34)
int(3)
int(101)
int(102)
int(103)
int(104)
int(1)
int(2)
int(3)
int(4)
