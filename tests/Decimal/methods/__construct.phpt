--TEST--
Decimal::__construct
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
new Decimal\Decimal;
?>
--EXPECTF--
Fatal error: Uncaught Error: Call to private Decimal\Decimal::__construct() from invalid context in %s:%d
Stack trace:
#0 {main}
  thrown in %s on line %d
