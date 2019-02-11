--TEST--
Number clone
--FILE--
<?php
require __DIR__ . "/helpers/Number.php";

var_dump(clone Number::valueOf(4));
?>
--EXPECT--
object(Number)#2 (1) {
  ["value":protected]=>
  int(4)
}
