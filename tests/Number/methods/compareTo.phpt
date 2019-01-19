--TEST--
Number::add
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
require __DIR__ . "/../include/Number.php";

use \Decimal\Decimal;
use \Decimal\Rational;

/* Number added to scalar by operator */
var_dump(Number::valueOf(4) <=> "2.5");

/* Scalar added to Number by operator */
var_dump("2.5" <=> Number::valueOf(4));

/* Number added to Number by operator */
var_dump(Number::valueOf(4) <=> Number::valueOf("2.5"));

/* Number added to Number by method */
var_dump(Number::valueOf(4)->compareTo(Number::valueOf("2.5")));

/* Number added to Decimal by operator */
var_dump(Number::valueOf(4) <=> Decimal::valueOf("2.5"));

/* Number added to Decimal by method */
var_dump(Number::valueOf(4)->compareTo(Decimal::valueOf("2.5")));

/* Number added to Rational by operator */
var_dump(Number::valueOf(4) <=> Rational::valueOf("2.5"));

/* Number added to Rational by method */
var_dump(Number::valueOf(4)->compareTo(Rational::valueOf("2.5")));

/* Decimal added to Number by operator */
var_dump(Decimal::valueOf(4) <=> Number::valueOf("2.5"));

/* Decimal added to Number by method */
var_dump(Decimal::valueOf(4)->compareTo(Number::valueOf("2.5")));

/* Rational added to Number by operator */
var_dump(Rational::valueOf(4) <=> Number::valueOf("2.5"));

/* Rational added to Number by method */
var_dump(Rational::valueOf(4)->compareTo(Number::valueOf("2.5")));

?>
--EXPECT--
Number::compareTo
int(1)
Number::compareTo
int(-1)
Number::compareTo
int(1)
Number::compareTo
int(1)
Number::compareTo
int(1)
Number::compareTo
int(1)
Number::compareTo
int(1)
Number::compareTo
int(1)
int(1)
int(1)
int(1)
int(1)
