--TEST--
Decimal::precision
--FILE--
<?php
use Decimal\Decimal;

var_dump(Decimal::valueOf(0)->precision());
var_dump(Decimal::valueOf(5)->precision());
var_dump(Decimal::valueOf(5, 5)->precision());

try {
	$x = Decimal::valueOf(0, 0)->precision();
} catch (\OutOfRangeException $e) {
	var_dump($x);
}
?>
--EXPECTF--
int(34)
int(34)
int(5)

Notice: Undefined variable: x in %s on line %d
NULL
