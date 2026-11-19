--TEST--
Decimal unserialize legacy C: format (PHP < 8.6)
--SKIPIF--
<?php if (PHP_VERSION_ID >= 80600) die("skip PHP < 8.6 only"); ?>
--FILE--
<?php
var_dump(unserialize('C:15:"Decimal\Decimal":26:{s:13:"abc0000000000";i:42;}'));
?>
--EXPECTF--
Warning: Class Decimal\Decimal has no unserializer in %s on line %d
object(Decimal\Decimal)#%d (0) {
}
