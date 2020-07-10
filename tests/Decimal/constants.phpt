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

var_dump(Decimal::PI);
var_dump(Decimal::valueOf(Decimal::PI));
var_dump(Decimal::E);
var_dump(Decimal::valueOf(Decimal::E));
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
string(17) "3.141592653589793"
object(Decimal\Decimal)#%d (2) {
  ["value"]=>
  string(17) "3.141592653589793"
  ["precision"]=>
  int(34)
}
string(36) "2.7182818284590452353602874713526625"
object(Decimal\Decimal)#%d (2) {
  ["value"]=>
  string(36) "2.7182818284590452353602874713526625"
  ["precision"]=>
  int(34)
}
