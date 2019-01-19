--TEST--
Number clone
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
require __DIR__ . "/include/Number.php";

var_dump(clone Number::valueOf(4));
?>
--EXPECT--
object(Number)#2 (1) {
  ["value":protected]=>
  int(4)
}
