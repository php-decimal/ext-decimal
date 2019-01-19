--TEST--
Rational object properties
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
use Decimal\Rational;

$instance = Rational::valueOf(0);

$instance->test = 5;
var_dump($instance->test);
unset($instance->test);
isset($instance->test);
var_dump($instance->test);

try {
    $instance[0] = 5;
} catch (Error $e) {
    printf("%s\n", $e->getMessage());
}

echo "Done";

?>
--EXPECTF--
Notice: Object properties are not supported in %s on line %d

Notice: Object properties are not supported in %s on line %d
NULL

Notice: Object properties are not supported in %s on line %d

Notice: Object properties are not supported in %s on line %d

Notice: Object properties are not supported in %s on line %d
NULL
Cannot use object of type Decimal\Rational as array
Done
