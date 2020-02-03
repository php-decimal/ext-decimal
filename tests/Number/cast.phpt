--TEST--
Number cast to string, bool, int, float
--FILE--
<?php
require __DIR__ . "/helpers/Number.php";

var_dump((string) Number::valueOf(4));
var_dump((bool)   Number::valueOf(0));     // Should always be true
var_dump((int)    Number::valueOf("2.9"));
var_dump((float)  Number::valueOf("2.9"));

?>
--EXPECT--
string(1) "4"
bool(true)
int(2)
float(2.9)