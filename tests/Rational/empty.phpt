--TEST--
Rational empty
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo 'skip';
?>
--FILE--
<?php
use Decimal\Rational;

var_dump(empty(Rational::valueOf(0)));
var_dump(empty(Rational::valueOf(1)));

var_dump(empty(Rational::valueOf( "1E-1000")));
var_dump(empty(Rational::valueOf("-1E-1000")));

var_dump(empty(Rational::valueOf( "NAN")));
var_dump(empty(Rational::valueOf( "INF")));
var_dump(empty(Rational::valueOf("-INF")));
?>
--EXPECT--
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
