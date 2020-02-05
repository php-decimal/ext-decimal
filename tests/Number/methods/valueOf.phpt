--TEST--
Number::valueOf
--FILE--
<?php
require __DIR__ . "/../helpers/Number.php";

var_dump(Number::valueOf("2.5"));
?>
--EXPECTF--
object(Number)#%d (1) {
  ["value":protected]=>
  string(3) "2.5"
}
