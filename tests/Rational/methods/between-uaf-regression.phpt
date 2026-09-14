--TEST--
Regression: Rational::between must not double-free its operands (use-after-free)
--FILE--
<?php
use Decimal\Rational;

$lo = Rational::valueOf("0");
$hi = Rational::valueOf("100");

for ($outer = 1; $outer <= 50; ++$outer) {
    $boundLow  = Rational::valueOf("1")->div(Rational::valueOf("2"));
    $boundHigh = Rational::valueOf("99")->mul(Rational::valueOf("1"));
    $needle    = Rational::valueOf("50");

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
