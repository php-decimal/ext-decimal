--TEST--
Number::sub
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
require __DIR__ . "/../include/Number.php";

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
