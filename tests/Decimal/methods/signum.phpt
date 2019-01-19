--TEST--
Decimal::signum
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
use Decimal\Decimal;

var_dump(Decimal::valueOf(0)->signum());

var_dump(Decimal::valueOf("1234.5678E+9")->signum());
var_dump(Decimal::valueOf("1234.5678E-9")->signum());

var_dump(Decimal::valueOf("-1234.5678E+9")->signum());
var_dump(Decimal::valueOf("-1234.5678E-9")->signum());

var_dump(Decimal::valueOf( "NAN")->signum());
var_dump(Decimal::valueOf( "INF")->signum());
var_dump(Decimal::valueOf("-INF")->signum());
?>
--EXPECT--
int(1)
int(1)
int(1)
int(-1)
int(-1)
int(1)
int(1)
int(-1)
