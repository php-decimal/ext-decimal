--TEST--
Decimal json_encode
--SKIPIF--
<?php
if (!extension_loaded("json")) echo 'skip';
?>
--FILE--
<?php
use Decimal\Decimal;

var_dump(Decimal::valueOf("1.2345", 20)->jsonSerialize());

var_dump(json_encode(Decimal::valueOf("1.2345", 20)));
var_dump(json_encode(Decimal::valueOf("5.0000", 20)));
?>
--EXPECT--
string(6) "1.2345"
string(8) ""1.2345""
string(8) ""5.0000""
