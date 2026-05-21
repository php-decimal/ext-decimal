--TEST--
Regression: Rational::equals must not double-free the operand (use-after-free)
--FILE--
<?php
use Decimal\Rational;

/*
 * Same root cause as the Decimal::equals UAF: zval_ptr_dtor() was called on
 * an argument obtained via Z_PARAM_ZVAL, which the VM frame already owns.
 */

$a = Rational::valueOf("1");
$b = Rational::valueOf("2");

for ($outer = 1; $outer <= 100; ++$outer) {
    $longLived = $a->div($b);
    for ($i = 0; $i < 100; ++$i) {
        $x = Rational::valueOf((string)($i + 1));
        $y = $x->div($b);
        $y->equals($longLived);
        $longLived->equals($y);
        unset($x, $y);
    }
    // Force heap churn so UAF can't hide behind reusable allocator slots.
    for ($j = 0; $j < 200; ++$j) {
        $junk = Rational::valueOf((string)$j);
    }
    unset($junk);
    if ((string)$longLived === "") {
        echo "empty cast at outer={$outer}\n";
        exit(1);
    }
}

echo "OK\n";
?>
--EXPECT--
OK
