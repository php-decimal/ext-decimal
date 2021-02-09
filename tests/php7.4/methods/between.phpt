--TEST--
Decimal::between
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

$tests = [
    [decimal(50),     0,              100,            true],
    [decimal(50),     decimal(0),     100,            true],
    [decimal(50),     0,              decimal(100),   true],
    [decimal(50),     decimal(0),     decimal(100),   true],
    [decimal(50),     50,             100,            true],
    [decimal(50),     decimal(50),    100,            true],
    [decimal(50),     50,             decimal(100),   true],
    [decimal(50),     decimal(50),    decimal(100),   true],
    [decimal(100),    50,             100,            true],
    [decimal(100),    decimal(50),    100,            true],
    [decimal(100),    50,             decimal(100),   true],
    [decimal(100),    decimal(50),    decimal(100),   true],
    [decimal(-10),    20,             100,            false],
    [decimal(-10),    decimal(20),    100,            false],
    [decimal(-10),    20,             decimal(100),   false],
    [decimal(-10),    decimal(20),    decimal(100),   false],
    [decimal(200),    20,             100,            false],
    [decimal(200),    decimal(20),    100,            false],
    [decimal(200),    20,             decimal(100),   false],
    [decimal(200),    decimal(20),    decimal(100),   false],


    [decimal("50.567"),     50.567,             100,                true],
    [decimal("50"),         decimal(0),         100,                true],
    [decimal("50"),         0,                  decimal(100),       true],
    [decimal("50"),         decimal(0),         decimal(100),       true],
    [decimal("50.567"),     "50.567",           100,                true],
    [decimal("50.567"),     decimal("50.567"),  100,                true],
    [decimal("50.567"),     "50.567",           decimal(100),       true],
    [decimal("50.567"),     decimal("50.567"),  decimal(100),       true],
    [decimal("50.566"),     "50.567",           100,                false],
    [decimal("50.566"),     decimal("50.567"),  100,                false],
    [decimal("50.566"),     "50.567",           decimal(100),       false],
    [decimal("50.566"),     decimal("50.567"),  decimal(100),       false],
    [decimal("99.998"),     50,                 99.998,             true],
    [decimal("99.998"),     decimal(50),        99.998,             true],
    [decimal("99.998"),     50,                 decimal("99.998"),  true],
    [decimal("99.998"),     decimal(50),        decimal("99.998"),  true],
    [decimal("99.999"),     50,                 99.998,             false],
    [decimal("99.999"),     decimal(50),        99.998,             false],
    [decimal("99.999"),     50,                 decimal("99.998"),  false],
    [decimal("99.999"),     decimal(50),        decimal("99.998"),  false],

];

foreach ($tests as $index => $test) {
    list($decimal, $opLeft, $opRight, $expect) = $test;

    $result = $decimal->between($opLeft, $opRight);
    if ($result !== $expect) {
        var_dump(compact("index", "decimal", "opLeft", "opRight", "expect", "result"));
        break;
    }
}

?>
--EXPECT--
