--TEST--
Decimal::parity
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
use Decimal\Decimal;

var_dump((string) Decimal::valueOf(0)->parity());

var_dump((string) Decimal::valueOf("0")->parity());
var_dump((string) Decimal::valueOf("1")->parity());
var_dump((string) Decimal::valueOf("2")->parity());
var_dump((string) Decimal::valueOf("3")->parity());

var_dump((string) Decimal::valueOf("-0")->parity());
var_dump((string) Decimal::valueOf("-1")->parity());
var_dump((string) Decimal::valueOf("-2")->parity());
var_dump((string) Decimal::valueOf("-3")->parity());

var_dump((string) Decimal::valueOf("0.5")->parity());
var_dump((string) Decimal::valueOf("1.5")->parity());
var_dump((string) Decimal::valueOf("2.5")->parity());
var_dump((string) Decimal::valueOf("3.5")->parity());

var_dump((string) Decimal::valueOf( "NAN")->parity());
var_dump((string) Decimal::valueOf( "INF")->parity());
var_dump((string) Decimal::valueOf("-INF")->parity());

?>
--EXPECT--
string(1) "0"
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
