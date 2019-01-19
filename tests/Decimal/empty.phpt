--TEST--
Decimal empty
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo 'skip';
?>
--FILE--
<?php
use Decimal\Decimal;

var_dump(empty(Decimal::valueOf(0)));
var_dump(empty(Decimal::valueOf(1)));

var_dump(empty(Decimal::valueOf( "1E-1000")));
var_dump(empty(Decimal::valueOf("-1E-1000")));

var_dump(empty(Decimal::valueOf( "NAN")));
var_dump(empty(Decimal::valueOf( "INF")));
var_dump(empty(Decimal::valueOf("-INF")));
?>
--EXPECT--
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
