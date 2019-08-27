--TEST--
Number::compareTo
--FILE--
<?php
require __DIR__ . "/../helpers/Number.php";

use \Decimal\Decimal;
use \Decimal\Rational;

var_dump(Number::valueOf(4) <=> "2.5"); //  1
var_dump("2.5" <=> Number::valueOf(4)); // -1
printf("\n");

var_dump(Number::valueOf(4) <=> 2.5); //  1
var_dump(2.5 <=> Number::valueOf(4)); // -1
printf("\n");

var_dump(Number::valueOf(4) <=> "4.0"); // 0
var_dump("4.0" <=> Number::valueOf(4)); // 0
printf("\n");

var_dump(Number::valueOf(4) <=> 4); // 0
var_dump(4 <=> Number::valueOf(4)); // 0
printf("\n");

var_dump(Number::valueOf(4) <=> 3); //  1
var_dump(3 <=> Number::valueOf(4)); // -1
printf("\n");

var_dump(Number::valueOf(4) <=> []); //  1
var_dump([] <=> Number::valueOf(4)); // -1
printf("\n");

var_dump(Number::valueOf(4) <=> true); // 0
var_dump(true <=> Number::valueOf(4)); // 0
printf("\n");

var_dump(Number::valueOf(4) <=> false); //  1
var_dump(false <=> Number::valueOf(4)); // -1
printf("\n");

var_dump(Number::valueOf(4) <=> 2.5);                       // 1
var_dump(Number::valueOf(4) <=> 3);                         // 1
var_dump(Number::valueOf(4) <=> '2.5');                     // 1
var_dump(Number::valueOf(4) <=> Number::valueOf("2.5"));    // 1
var_dump(Number::valueOf(4) <=> Decimal::valueOf("2.5"));   // 1
var_dump(Number::valueOf(4) <=> Rational::valueOf("2.5"));  // 1
var_dump(Decimal::valueOf(4) <=> Number::valueOf("2.5"));   // 1
var_dump(Rational::valueOf(4) <=> Number::valueOf("2.5"));  // 1
printf("\n");

var_dump(Number::valueOf(4)->compareTo(2.5));                       // 1
var_dump(Number::valueOf(4)->compareTo(3));                         // 1
var_dump(Number::valueOf(4)->compareTo('2.5'));                     // 1
var_dump(Number::valueOf(4)->compareTo(Number::valueOf("2.5")));    // 1
var_dump(Number::valueOf(4)->compareTo(Decimal::valueOf("2.5")));   // 1
var_dump(Number::valueOf(4)->compareTo(Rational::valueOf("2.5")));  // 1
var_dump(Decimal::valueOf(4)->compareTo(Number::valueOf("2.5")));   // 1
var_dump(Rational::valueOf(4)->compareTo(Number::valueOf("2.5")));  // 1

?>
--EXPECT--
int(1)
int(-1)

int(1)
int(-1)

int(0)
int(0)

int(0)
int(0)

int(1)
int(-1)

int(1)
int(-1)

int(0)
int(0)

int(1)
int(-1)

int(1)
int(1)
int(1)
Number::compareTo
int(1)
Number::compareTo
int(1)
Number::compareTo
int(1)
Number::toDecimal
int(1)
Number::toRational
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
Number::compareTo
int(1)
Number::toDecimal
int(1)
Number::toRational
int(1)