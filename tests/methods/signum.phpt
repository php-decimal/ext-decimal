--TEST--
Decimal::signum
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
use Decimal\Decimal;

/**
 * Shortcut to construct a new decimal.
 */
function decimal(...$args) { return new Decimal(...$args); }

var_dump(decimal()->signum());
var_dump(decimal("-0")->signum());
var_dump(decimal("+0")->signum());

var_dump(decimal("1234.5678E+9")->signum());
var_dump(decimal("1234.5678E-9")->signum());

var_dump(decimal("-1234.5678E+9")->signum());
var_dump(decimal("-1234.5678E-9")->signum());

var_dump(decimal( "INF")->signum());
var_dump(decimal("-INF")->signum());

try {
    decimal("NAN")->signum();
} catch (RuntimeException $e) {
    printf("%s\n", $e->getMessage());
}

?>
--EXPECT--
int(0)
int(0)
int(0)
int(1)
int(1)
int(-1)
int(-1)
int(1)
int(-1)
Sign of NaN is not defined
