--TEST--
Decimal::signum
--FILE--
<?php
use Decimal\Decimal;

var_dump(Decimal::valueOf(0)->signum());
var_dump(Decimal::valueOf("+0")->signum());
var_dump(Decimal::valueOf("-0")->signum());

var_dump(Decimal::valueOf("+0.1")->signum());
var_dump(Decimal::valueOf("-0.1")->signum());

var_dump(Decimal::valueOf("1234.5678E+9")->signum());
var_dump(Decimal::valueOf("1234.5678E-9")->signum());

var_dump(Decimal::valueOf("-1234.5678E+9")->signum());
var_dump(Decimal::valueOf("-1234.5678E-9")->signum());

var_dump(Decimal::valueOf( "INF")->signum());
var_dump(Decimal::valueOf("-INF")->signum());

try {
    Decimal::valueOf("NAN")->signum();
} catch (RuntimeException $e) {
    printf("%s\n", $e->getMessage());
}

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
int(-1)
Sign of NaN is not defined
