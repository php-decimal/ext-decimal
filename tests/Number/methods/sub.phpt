--TEST--
Number::sub
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
require __DIR__ . "/../helpers/Number.php";

use \Decimal\Decimal;
use \Decimal\Rational;

/* Number minus scalar by operator */
var_dump(Number::valueOf(4) - "2.5");

/* Scalar minus Number by operator */
var_dump("2.5" - Number::valueOf(4));

/* Number minus Number by operator */
var_dump(Number::valueOf(4) - Number::valueOf("2.5"));

/* Number minus Number by method */
var_dump(Number::valueOf(4)->sub(Number::valueOf("2.5")));

/* Number minus Decimal by operator */
var_dump(Number::valueOf(4) - Decimal::valueOf("2.5"));

/* Number minus Decimal by method */
var_dump(Number::valueOf(4)->sub(Decimal::valueOf("2.5")));

/* Number minus Rational by operator */
var_dump(Number::valueOf(4) - Rational::valueOf("2.5"));

/* Number minus Rational by method */
var_dump(Number::valueOf(4)->sub(Rational::valueOf("2.5")));

/* Decimal minus Number by operator */
var_dump(Decimal::valueOf(4) - Number::valueOf("2.5"));

/* Decimal minus Number by method */
var_dump(Decimal::valueOf(4)->sub(Number::valueOf("2.5")));

/* Rational minus Number by operator */
var_dump(Rational::valueOf(4) - Number::valueOf("2.5"));

/* Rational minus Number by method */
var_dump(Rational::valueOf(4)->sub(Number::valueOf("2.5")));

?>
--EXPECT--
Number::sub
object(Number)#2 (1) {
  ["value":protected]=>
  float(1.5)
}
Number::sub
object(Number)#3 (1) {
  ["value":protected]=>
  float(-1.5)
}
Number::sub
object(Number)#1 (1) {
  ["value":protected]=>
  float(1.5)
}
Number::sub
object(Number)#3 (1) {
  ["value":protected]=>
  float(1.5)
}
Number::sub
object(Number)#2 (1) {
  ["value":protected]=>
  object(Decimal\Decimal)#4 (2) {
    ["value"]=>
    string(3) "1.5"
    ["precision"]=>
    int(34)
  }
}
Number::sub
object(Number)#1 (1) {
  ["value":protected]=>
  object(Decimal\Decimal)#3 (2) {
    ["value"]=>
    string(3) "1.5"
    ["precision"]=>
    int(34)
  }
}
Number::sub
object(Number)#2 (1) {
  ["value":protected]=>
  object(Decimal\Rational)#4 (2) {
    ["num"]=>
    string(1) "3"
    ["den"]=>
    string(1) "2"
  }
}
Number::sub
object(Number)#3 (1) {
  ["value":protected]=>
  object(Decimal\Rational)#1 (2) {
    ["num"]=>
    string(1) "3"
    ["den"]=>
    string(1) "2"
  }
}
Number::toDecimal
object(Decimal\Decimal)#2 (2) {
  ["value"]=>
  string(3) "1.5"
  ["precision"]=>
  int(34)
}
Number::toDecimal
object(Decimal\Decimal)#2 (2) {
  ["value"]=>
  string(3) "1.5"
  ["precision"]=>
  int(34)
}
Number::toRational
object(Decimal\Rational)#3 (2) {
  ["num"]=>
  string(1) "3"
  ["den"]=>
  string(1) "2"
}
Number::toRational
object(Decimal\Rational)#2 (2) {
  ["num"]=>
  string(1) "3"
  ["den"]=>
  string(1) "2"
}
