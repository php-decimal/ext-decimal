--TEST--
Rational operators
--FILE--
<?php
use Decimal\Rational;

/**
 * Arithmetic and comparison operators are tested as part of their corresponding
 * method, for example, "+" will be tested by "add"
 */

/**
 * Test that an assign op does not modify the original the original.
 */
function times2(Rational $a) {
    return $a *= 2;
}

$x = Rational::valueOf("1E+8");

print_r(times2($x));
print_r($x);

try {
    var_dump($x | 1); /* Not supported */
} catch (ArithmeticError $e) {
    printf("%s\n", $e->getMessage());
}
?>
--EXPECT--
Decimal\Rational Object
(
    [num] => 200000000
    [den] => 1
)
Decimal\Rational Object
(
    [num] => 100000000
    [den] => 1
)
Operator not supported by decimal numbers
