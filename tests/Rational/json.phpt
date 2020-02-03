--TEST--
Rational json_encode
--SKIPIF--
<?php
if (!extension_loaded("json")) echo 'skip';
?>
--FILE--
<?php
use Decimal\Rational;

var_dump(json_encode(Rational::valueOf("1.2345")));
var_dump(json_encode(Rational::valueOf("5.0000")));
?>
--EXPECT--
string(12) ""2469\/2000""
string(3) ""5""
