--TEST--
Number empty
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
require __DIR__ . "/helpers/Number.php";

var_dump(empty(Number::valueOf(0)));
var_dump(empty(Number::valueOf(1)));

?>
--EXPECT--
bool(false)
bool(false)
