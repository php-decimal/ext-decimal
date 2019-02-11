--TEST--
Decimal operators
--FILE--
<?php
use Decimal\Decimal;

/**
 * Arithmetic and comparison operators are tested as part of their corresponding
 * method, for example, "+" will be tested by "add"
 */

/**
 * Test that an assign op does not modify the original the original.
 */
function times2(Decimal $a) {
    return $a *= 2;
}

$x = Decimal::valueOf("1E+8", 12);

print_r(times2($x));
print_r($x);

try {
    var_dump($x | 1); /* Not supported */
} catch (ArithmeticError $e) {
    printf("%s\n", $e->getMessage());
}
?>
--EXPECT--
Decimal\Decimal Object
(
    [value] => 200000000
    [precision] => 12
)
Decimal\Decimal Object
(
    [value] => 100000000
    [precision] => 12
)
Operator not supported by decimal numbers
