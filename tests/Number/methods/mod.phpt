--TEST--
Number::mod
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
require __DIR__ . "/../helpers/Number.php";

use \Decimal\Decimal;
use \Decimal\Rational;

/* Number mod scalar using operator */
var_dump(Number::valueOf(9) % "6.5");

/* Scalar mod Number using operator */
var_dump("6.5" % Number::valueOf(9));

/* Number mod Number using operator */
var_dump(Number::valueOf(9) % Number::valueOf("6.5"));

/* Number mod Number using method */
var_dump(Number::valueOf(9)->mod(Number::valueOf("6.5")));

/* Number mod Decimal using operator */
var_dump(Number::valueOf(9) % Decimal::valueOf("6.5"));

/* Number mod Decimal using method */
var_dump(Number::valueOf(9)->mod(Decimal::valueOf("6.5")));

/* Number mod Rational using operator */
var_dump(Number::valueOf(9) % Rational::valueOf("6.5"));

/* Number mod Rational using method */
var_dump(Number::valueOf(9)->mod(Rational::valueOf("6.5")));

/* Decimal mod Number using operator */
var_dump(Decimal::valueOf(9) % Number::valueOf("6.5"));

/* Decimal mod Number using method */
var_dump(Decimal::valueOf(9)->mod(Number::valueOf("6.5")));

/* Rational mod Number using operator */
var_dump(Rational::valueOf(9) % Number::valueOf("6.5"));

/* Rational mod Number using method */
var_dump(Rational::valueOf(9)->mod(Number::valueOf("6.5")));

?>
--EXPECT--
Number::mod
object(Number)#2 (1) {
  ["value":protected]=>
  int(3)
}
Number::mod
object(Number)#3 (1) {
  ["value":protected]=>
  int(6)
}
Number::mod
object(Number)#1 (1) {
  ["value":protected]=>
  int(3)
}
Number::mod
object(Number)#3 (1) {
  ["value":protected]=>
  int(3)
}
Number::mod
object(Number)#2 (1) {
  ["value":protected]=>
  object(Decimal\Decimal)#4 (2) {
    ["value"]=>
    string(1) "3"
    ["precision"]=>
    int(34)
  }
}
Number::mod
object(Number)#1 (1) {
  ["value":protected]=>
  object(Decimal\Decimal)#3 (2) {
    ["value"]=>
    string(1) "3"
    ["precision"]=>
    int(34)
  }
}
Number::mod
object(Number)#2 (1) {
  ["value":protected]=>
  object(Decimal\Rational)#4 (2) {
    ["num"]=>
    string(1) "3"
    ["den"]=>
    string(1) "1"
  }
}
Number::mod
object(Number)#3 (1) {
  ["value":protected]=>
  object(Decimal\Rational)#1 (2) {
    ["num"]=>
    string(1) "3"
    ["den"]=>
    string(1) "1"
  }
}
Number::toDecimal
object(Decimal\Decimal)#2 (2) {
  ["value"]=>
  string(1) "3"
  ["precision"]=>
  int(34)
}
Number::toDecimal
object(Decimal\Decimal)#3 (2) {
  ["value"]=>
  string(1) "3"
  ["precision"]=>
  int(34)
}
Number::toRational
object(Decimal\Rational)#1 (2) {
  ["num"]=>
  string(1) "3"
  ["den"]=>
  string(1) "1"
}
Number::toRational
object(Decimal\Rational)#3 (2) {
  ["num"]=>
  string(1) "3"
  ["den"]=>
  string(1) "1"
}
