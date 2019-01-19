--TEST--
Number::valueOf
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
require __DIR__ . "/../include/Number.php";

var_dump(Number::valueOf("2.5"));
?>
--EXPECT--
object(Number)#1 (1) {
  ["value":protected]=>
  string(3) "2.5"
}
