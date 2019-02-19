--TEST--
Number::div
--FILE--
<?php
require __DIR__ . "/../helpers/Number.php";

use \Decimal\Decimal;
use \Decimal\Rational;

/* Number divided by scalar using operator */
var_dump(Number::valueOf(4) / "2.5");

/* Scalar divided by Number using operator */
var_dump("2.5" / Number::valueOf(4));

/* Number divided by Number using operator */
var_dump(Number::valueOf(4) / Number::valueOf("2.5"));

/* Number divided by Number using method */
var_dump(Number::valueOf(4)->div(Number::valueOf("2.5")));

/* Number divided by Decimal using operator */
var_dump(Number::valueOf(4) / Decimal::valueOf("2.5"));

/* Number divided by Decimal using method */
var_dump(Number::valueOf(4)->div(Decimal::valueOf("2.5")));

/* Number divided by Rational using operator */
var_dump(Number::valueOf(4) / Rational::valueOf("2.5"));

/* Number divided by Rational using method */
var_dump(Number::valueOf(4)->div(Rational::valueOf("2.5")));

/* Decimal divided by Number using operator */
var_dump(Decimal::valueOf(4) / Number::valueOf("2.5"));

/* Decimal divided by Number using method */
var_dump(Decimal::valueOf(4)->div(Number::valueOf("2.5")));

/* Rational divided by Number using operator */
var_dump(Rational::valueOf(4) / Number::valueOf("2.5"));

/* Rational divided by Number using method */
var_dump(Rational::valueOf(4)->div(Number::valueOf("2.5")));

?>
--EXPECT--
Number::div
object(Number)#2 (1) {
  ["value":protected]=>
  float(1.6)
}
Number::div
object(Number)#3 (1) {
  ["value":protected]=>
  float(0.625)
}
Number::div
object(Number)#1 (1) {
  ["value":protected]=>
  float(1.6)
}
Number::div
object(Number)#3 (1) {
  ["value":protected]=>
  float(1.6)
}
Number::div
object(Number)#2 (1) {
  ["value":protected]=>
  object(Decimal\Decimal)#4 (2) {
    ["value"]=>
    string(3) "1.6"
    ["precision"]=>
    int(34)
  }
}
Number::div
object(Number)#1 (1) {
  ["value":protected]=>
  object(Decimal\Decimal)#3 (2) {
    ["value"]=>
    string(3) "1.6"
    ["precision"]=>
    int(34)
  }
}
Number::div
object(Number)#2 (1) {
  ["value":protected]=>
  object(Decimal\Rational)#4 (2) {
    ["num"]=>
    string(1) "8"
    ["den"]=>
    string(1) "5"
  }
}
Number::div
object(Number)#3 (1) {
  ["value":protected]=>
  object(Decimal\Rational)#1 (2) {
    ["num"]=>
    string(1) "8"
    ["den"]=>
    string(1) "5"
  }
}
Number::toDecimal
object(Decimal\Decimal)#2 (2) {
  ["value"]=>
  string(3) "1.6"
  ["precision"]=>
  int(34)
}
Number::toDecimal
object(Decimal\Decimal)#2 (2) {
  ["value"]=>
  string(3) "1.6"
  ["precision"]=>
  int(34)
}
Number::toRational
object(Decimal\Rational)#3 (2) {
  ["num"]=>
  string(1) "8"
  ["den"]=>
  string(1) "5"
}
Number::toRational
object(Decimal\Rational)#3 (2) {
  ["num"]=>
  string(1) "8"
  ["den"]=>
  string(1) "5"
}
