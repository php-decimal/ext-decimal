--TEST--
Rational::signum
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
use Decimal\Rational;

var_dump(Rational::valueOf(0)->signum());
var_dump(Rational::valueOf("+0")->signum());
var_dump(Rational::valueOf("-0")->signum());

var_dump(Rational::valueOf("+0.1")->signum());
var_dump(Rational::valueOf("-0.1")->signum());

var_dump(Rational::valueOf("1234.5678E+9")->signum());
var_dump(Rational::valueOf("1234.5678E-9")->signum());

var_dump(Rational::valueOf("-1234.5678E+9")->signum());
var_dump(Rational::valueOf("-1234.5678E-9")->signum());

var_dump(Rational::valueOf( "NAN")->signum());
var_dump(Rational::valueOf( "INF")->signum());
var_dump(Rational::valueOf("-INF")->signum());
?>
--EXPECT--
int(0)
int(0)
int(0)
int(1)
int(-1)
int(1)
int(1)
int(-1)
int(-1)
int(1)
int(1)
int(-1)
