--TEST--
Decimal::trunc
--FILE--
<?php
use Decimal\Decimal;

var_dump((string) Decimal::valueOf(0)->trunc());

var_dump((string) Decimal::valueOf("1234.5678")->trunc());
var_dump((string) Decimal::valueOf("1E-500")->trunc());

var_dump((string) Decimal::valueOf( "NAN")->trunc());
var_dump((string) Decimal::valueOf( "INF")->trunc());
var_dump((string) Decimal::valueOf("-INF")->trunc());

/**
 * Test immutable
 */
$obj = Decimal::valueOf("1.234");
$obj->trunc();
var_dump((string) $obj);

?>
--EXPECT--
string(1) "0"
string(4) "1234"
string(1) "0"
string(3) "NAN"
string(3) "INF"
string(4) "-INF"
string(5) "1.234"
