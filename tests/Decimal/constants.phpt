--TEST--
Decimal constants
--FILE--
<?php
use Decimal\Decimal;

var_dump(Decimal::DEFAULT_PRECISION);
var_dump(Decimal::DEFAULT_ROUNDING);

var_dump(Decimal::ROUND_UP);
var_dump(Decimal::ROUND_DOWN);
var_dump(Decimal::ROUND_CEILING);
var_dump(Decimal::ROUND_FLOOR);
var_dump(Decimal::ROUND_HALF_UP);
var_dump(Decimal::ROUND_HALF_DOWN);
var_dump(Decimal::ROUND_HALF_EVEN);
var_dump(Decimal::ROUND_HALF_ODD);

var_dump(Decimal::MIN_PRECISION);
var_dump(Decimal::MAX_PRECISION);
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
