--TEST--
Number::add
--FILE--
<?php
require __DIR__ . "/../helpers/Number.php";

use \Decimal\Decimal;
use \Decimal\Rational;

/* Number added to scalar by operator */
var_dump(Number::valueOf(4) + "2.5");

/* Scalar added to Number by operator */
var_dump("2.5" + Number::valueOf(4));

/* Number added to Number by operator */
var_dump(Number::valueOf(4) + Number::valueOf("2.5"));

/* Number added to Number by method */
var_dump(Number::valueOf(4)->add(Number::valueOf("2.5")));

/* Number added to Decimal by operator */
var_dump(Number::valueOf(4) + Decimal::valueOf("2.5"));

/* Number added to Decimal by method */
var_dump(Number::valueOf(4)->add(Decimal::valueOf("2.5")));

/* Number added to Rational by operator */
var_dump(Number::valueOf(4) + Rational::valueOf("2.5"));

/* Number added to Rational by method */
var_dump(Number::valueOf(4)->add(Rational::valueOf("2.5")));

/* Decimal added to Number by operator */
var_dump(Decimal::valueOf(4) + Number::valueOf("2.5"));

/* Decimal added to Number by method */
var_dump(Decimal::valueOf(4)->add(Number::valueOf("2.5")));

/* Rational added to Number by operator */
var_dump(Rational::valueOf(4) + Number::valueOf("2.5"));

/* Rational added to Number by method */
var_dump(Rational::valueOf(4)->add(Number::valueOf("2.5")));

?>
--EXPECT--
Number::add
object(Number)#2 (1) {
  ["value":protected]=>
  float(6.5)
}
Number::add
object(Number)#3 (1) {
  ["value":protected]=>
  float(6.5)
}
Number::add
object(Number)#1 (1) {
  ["value":protected]=>
  float(6.5)
}
Number::add
object(Number)#3 (1) {
  ["value":protected]=>
  float(6.5)
}
Number::add
object(Number)#2 (1) {
  ["value":protected]=>
  object(Decimal\Decimal)#4 (2) {
    ["value"]=>
    string(3) "6.5"
    ["precision"]=>
    int(34)
  }
}
Number::add
object(Number)#1 (1) {
  ["value":protected]=>
  object(Decimal\Decimal)#3 (2) {
    ["value"]=>
    string(3) "6.5"
    ["precision"]=>
    int(34)
  }
}
Number::add
object(Number)#2 (1) {
  ["value":protected]=>
  object(Decimal\Rational)#4 (2) {
    ["num"]=>
    string(2) "13"
    ["den"]=>
    string(1) "2"
  }
}
Number::add
object(Number)#3 (1) {
  ["value":protected]=>
  object(Decimal\Rational)#1 (2) {
    ["num"]=>
    string(2) "13"
    ["den"]=>
    string(1) "2"
  }
}
Number::toDecimal
object(Decimal\Decimal)#2 (2) {
  ["value"]=>
  string(3) "6.5"
  ["precision"]=>
  int(34)
}
Number::toDecimal
object(Decimal\Decimal)#2 (2) {
  ["value"]=>
  string(3) "6.5"
  ["precision"]=>
  int(34)
}
Number::toRational
object(Decimal\Rational)#3 (2) {
  ["num"]=>
  string(2) "13"
  ["den"]=>
  string(1) "2"
}
Number::toRational
object(Decimal\Rational)#3 (2) {
  ["num"]=>
  string(2) "13"
  ["den"]=>
  string(1) "2"
}
