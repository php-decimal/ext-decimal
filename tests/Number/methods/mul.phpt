--TEST--
Number::mul
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
require __DIR__ . "/../helpers/Number.php";

use \Decimal\Decimal;
use \Decimal\Rational;

/* Number multiplied by scalar using operator */
var_dump(Number::valueOf(4) * "2.5");

/* Scalar multiplied by Number using operator */
var_dump("2.5" * Number::valueOf(4));

/* Number multiplied by Number using operator */
var_dump(Number::valueOf(4) * Number::valueOf("2.5"));

/* Number multiplied by Number using method */
var_dump(Number::valueOf(4)->mul(Number::valueOf("2.5")));

/* Number multiplied by Decimal using operator */
var_dump(Number::valueOf(4) * Decimal::valueOf("2.5"));

/* Number multiplied by Decimal using method */
var_dump(Number::valueOf(4)->mul(Decimal::valueOf("2.5")));

/* Number multiplied by Rational using operator */
var_dump(Number::valueOf(4) * Rational::valueOf("2.5"));

/* Number multiplied by Rational using method */
var_dump(Number::valueOf(4)->mul(Rational::valueOf("2.5")));

/* Decimal multiplied by Number using operator */
var_dump(Decimal::valueOf(4) * Number::valueOf("2.5"));

/* Decimal multiplied by Number using method */
var_dump(Decimal::valueOf(4)->mul(Number::valueOf("2.5")));

/* Rational multiplied by Number using operator */
var_dump(Rational::valueOf(4) * Number::valueOf("2.5"));

/* Rational multiplied by Number using method */
var_dump(Rational::valueOf(4)->mul(Number::valueOf("2.5")));

?>
--EXPECT--
Number::mul
object(Number)#2 (1) {
  ["value":protected]=>
  float(10)
}
Number::mul
object(Number)#3 (1) {
  ["value":protected]=>
  float(10)
}
Number::mul
object(Number)#1 (1) {
  ["value":protected]=>
  float(10)
}
Number::mul
object(Number)#3 (1) {
  ["value":protected]=>
  float(10)
}
Number::mul
object(Number)#2 (1) {
  ["value":protected]=>
  object(Decimal\Decimal)#4 (2) {
    ["value"]=>
    string(4) "10.0"
    ["precision"]=>
    int(34)
  }
}
Number::mul
object(Number)#1 (1) {
  ["value":protected]=>
  object(Decimal\Decimal)#3 (2) {
    ["value"]=>
    string(4) "10.0"
    ["precision"]=>
    int(34)
  }
}
Number::mul
object(Number)#2 (1) {
  ["value":protected]=>
  object(Decimal\Rational)#4 (2) {
    ["num"]=>
    string(2) "10"
    ["den"]=>
    string(1) "1"
  }
}
Number::mul
object(Number)#3 (1) {
  ["value":protected]=>
  object(Decimal\Rational)#1 (2) {
    ["num"]=>
    string(2) "10"
    ["den"]=>
    string(1) "1"
  }
}
Number::toDecimal
object(Decimal\Decimal)#2 (2) {
  ["value"]=>
  string(4) "10.0"
  ["precision"]=>
  int(34)
}
Number::toDecimal
object(Decimal\Decimal)#3 (2) {
  ["value"]=>
  string(4) "10.0"
  ["precision"]=>
  int(34)
}
Number::toRational
object(Decimal\Rational)#1 (2) {
  ["num"]=>
  string(2) "10"
  ["den"]=>
  string(1) "1"
}
Number::toRational
object(Decimal\Rational)#3 (2) {
  ["num"]=>
  string(2) "10"
  ["den"]=>
  string(1) "1"
}
