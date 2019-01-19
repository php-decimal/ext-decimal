--TEST--
Number constants
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo 'skip';
?>
--FILE--
<?php
use Decimal\Number;

var_dump(Number::DEFAULT_PRECISION);
var_dump(Number::DEFAULT_ROUNDING);

var_dump(Number::ROUND_UP);
var_dump(Number::ROUND_DOWN);
var_dump(Number::ROUND_CEILING);
var_dump(Number::ROUND_FLOOR);
var_dump(Number::ROUND_HALF_UP);
var_dump(Number::ROUND_HALF_DOWN);
var_dump(Number::ROUND_HALF_EVEN);
var_dump(Number::ROUND_HALF_ODD);

var_dump(Number::MIN_PRECISION);
var_dump(Number::MAX_PRECISION);
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
int(1)
int(%d)
