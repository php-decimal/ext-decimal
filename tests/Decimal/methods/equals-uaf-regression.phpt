--TEST--
Regression: Decimal::equals must not double-free the operand (use-after-free)
--FILE--
<?php
use Decimal\Decimal;

/*
 * On the unpatched build, Decimal::equals() did:
 *     Z_PARAM_ZVAL(other)   -- non-separated, non-addref'd pointer
 *     ... php_decimal_compare(...) ...
 *     zval_ptr_dtor(other)  -- BUG: decrements refcount of an arg the VM
 *                              still owns. After the frame cleans up the arg
 *                              once more, the operand is freed too early
 *                              => UAF on the caller's variable.
 *
 * Pattern: create a long-lived Decimal whose only ref is the local variable,
 * call equals() on it from many short-lived peers, then keep using it.
 * On an unpatched build this reliably SIGSEGVs.
 */

$vol = Decimal::valueOf("1.02");
$mul = Decimal::valueOf("0.45");

for ($outer = 1; $outer <= 50; ++$outer) {
    $longLived = Decimal::valueOf("15.99")->div($vol)->mul($mul);
    for ($i = 0; $i < 60; ++$i) {
        $x = Decimal::valueOf((string)($i + 1));
        $y = $x->div($vol)->mul($mul);
        $y->equals($longLived);
        $longLived->equals($y);
    }
    // Use long-lived after equals(): trips UAF on broken build.
    if ((string)$longLived === "") {
        echo "empty cast at outer={$outer}\n";
        exit(1);
    }
}

echo "OK\n";
?>
--EXPECT--
OK
