--TEST--
Number::shiftl
--FILE--
<?php
require __DIR__ . "/../helpers/Number.php";

var_dump(Number::valueOf(4) <<  2);
var_dump(Number::valueOf(4) << -2);

var_dump(Number::valueOf(4) << "1");

var_dump(Number::valueOf(4)->shiftl( 2));
var_dump(Number::valueOf(4)->shiftl(-2));

var_dump(Number::valueOf(4)->shiftl("1"));

?>
--EXPECT--
Number::shiftl
object(Number)#2 (1) {
  ["value":protected]=>
  int(400)
}
Number::shiftl
object(Number)#1 (1) {
  ["value":protected]=>
  float(0.04)
}
Number::shiftl
object(Number)#2 (1) {
  ["value":protected]=>
  int(40)
}
Number::shiftl
object(Number)#1 (1) {
  ["value":protected]=>
  int(400)
}
Number::shiftl
object(Number)#2 (1) {
  ["value":protected]=>
  float(0.04)
}
Number::shiftl
object(Number)#1 (1) {
  ["value":protected]=>
  int(40)
}
