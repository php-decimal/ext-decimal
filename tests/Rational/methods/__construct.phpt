--TEST--
Rational::__construct
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
new Decimal\Rational;
?>
--EXPECTF--
Fatal error: Uncaught Error: Call to private Decimal\Rational::__construct() from invalid context in %s:%d
Stack trace:
#0 {main}
  thrown in %s on line %d
