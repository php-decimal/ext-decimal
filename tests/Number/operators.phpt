--TEST--
Decimal operators
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo 'skip';
?>
--FILE--
<?php
/**
 * Arithmetic and comparison operators are tested as part of their corresponding
 * method, for example, "+" will be tested by "add"
 */
require __DIR__ . "/helpers/Number.php";

/**
 * Test that an assign op does not modify the original the original.
 */
function times2(Number $a) {
    return $a *= 2;
}

$x = Number::valueOf("1E+8");

print_r(times2($x));
print_r($x);

try {
    var_dump($x | 1); /* Not supported */
} catch (ArithmeticError $e) {
    printf("%s\n", $e->getMessage());
}
?>
--EXPECT--
Number::mul
Number Object
(
    [value:protected] => 200000000
)
Number Object
(
    [value:protected] => 1E+8
)
Operator not supported by decimal numbers
