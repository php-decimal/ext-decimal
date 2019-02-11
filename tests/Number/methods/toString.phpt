--TEST--
Number::sub
--FILE--
<?php
require __DIR__ . "/../helpers/Number.php";

var_dump(Number::valueOf(4)->toString());
var_dump(Number::valueOf(4)->__toString());
var_dump((string) Number::valueOf(4));

/* Check that we autbox the string, rather than cast the Number */
var_dump(Number::valueOf(4) + "1");

?>
--EXPECT--
Number::toString
string(1) "4"
Number::toString
string(1) "4"
Number::toString
string(1) "4"
Number::add
object(Number)#2 (1) {
  ["value":protected]=>
  int(5)
}
