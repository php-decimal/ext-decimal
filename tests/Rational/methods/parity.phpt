--TEST--
Rational::parity
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
use Decimal\Rational;

var_dump((string) Rational::valueOf("0")->parity());
var_dump((string) Rational::valueOf("1")->parity());
var_dump((string) Rational::valueOf("2")->parity());
var_dump((string) Rational::valueOf("3")->parity());

var_dump((string) Rational::valueOf("-0")->parity());
var_dump((string) Rational::valueOf("-1")->parity());
var_dump((string) Rational::valueOf("-2")->parity());
var_dump((string) Rational::valueOf("-3")->parity());

var_dump((string) Rational::valueOf("0.5")->parity());
var_dump((string) Rational::valueOf("1.5")->parity());
var_dump((string) Rational::valueOf("2.5")->parity());
var_dump((string) Rational::valueOf("3.5")->parity());

var_dump((string) Rational::valueOf( "NAN")->parity());
var_dump((string) Rational::valueOf( "INF")->parity());
var_dump((string) Rational::valueOf("-INF")->parity());

?>
--EXPECT--
string(1) "0"
string(1) "1"
string(1) "0"
string(1) "1"
string(1) "0"
string(1) "1"
string(1) "0"
string(1) "1"
string(1) "0"
string(1) "1"
string(1) "0"
string(1) "1"
string(1) "1"
string(1) "1"
string(1) "1"
