--TEST--
Regression: Decimal::between must not double-free its operands (use-after-free)
--FILE--
<?php
use Decimal\Decimal;

/*
 * Decimal::between() called zval_ptr_dtor() on BOTH args (a and b) obtained
 * via Z_PARAM_ZVAL. Each call dropped one extra refcount for each operand.
 * Reproduce by holding the bounds only in fresh locals, calling between()
 * many times, then using the bounds afterwards.
 */

$lo = Decimal::valueOf("0");
$hi = Decimal::valueOf("100");

for ($outer = 1; $outer <= 50; ++$outer) {
    $boundLow  = Decimal::valueOf("1.23")->div(Decimal::valueOf("2"));
    $boundHigh = Decimal::valueOf("99.77")->mul(Decimal::valueOf("1"));
    $needle    = Decimal::valueOf("50");

    for ($i = 0; $i < 60; ++$i) {
        $needle->between($boundLow, $boundHigh);
        $needle->between($lo, $hi);
    }

    if (!$needle->between($boundLow, $boundHigh)) {
        echo "FAIL at outer={$outer}\n";
        exit(1);
    }
    if ((string)$boundLow === "" || (string)$boundHigh === "") {
        echo "empty cast at outer={$outer}\n";
        exit(1);
    }
}

echo "OK\n";
?>
--EXPECT--
OK