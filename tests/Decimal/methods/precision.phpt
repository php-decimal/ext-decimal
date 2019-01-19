--TEST--
Decimal::precision
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
use Decimal\Decimal;

var_dump(Decimal::valueOf(0)->precision());
var_dump(Decimal::valueOf(5)->precision());
var_dump(Decimal::valueOf(5, 5)->precision());

Decimal::valueOf(0, 0);
?>
--EXPECTF--
int(34)
int(34)
int(5)

Fatal error: Uncaught OutOfRangeException: Decimal precision out of range in %s:%d
Stack trace:
#0 %s(%d): Decimal\Decimal::valueOf(0, 0)
#1 {main}
  thrown in %s on line %d
