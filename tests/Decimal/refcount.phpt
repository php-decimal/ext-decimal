--TEST--
Decimal reference counting
--FILE--
<?php
$a = "1";
$b = "2";

/**
 *  This should reuse the decimal created for A as the result.
 */
$c = \Decimal\Decimal::valueOf($a, 5)->add($b)->sub(0);
debug_zval_dump($c);
// object(Decimal\Decimal)#1 (2) refcount(2){
//   ["value"]=>
//   string(1) "3" refcount(1)
//   ["precision"]=>
//   int(34)
// }


/**
 * This should reuse $c as the result.
 */
$c += "3";
debug_zval_dump($c);
// object(Decimal\Decimal)#1 (2) refcount(2){
//   ["value"]=>
//   string(1) "6" refcount(1)
//   ["precision"]=>
//   int(34)
// }


/**
 * This should NOT reuse $c because we evaluate the RHS before we assign.
 * The increment is rewritten as `$c = $c + 1`.
 */
$c++;
debug_zval_dump($c);
// object(Decimal\Decimal)#2 (2) refcount(2){
//   ["value"]=>
//   string(1) "7" refcount(1)
//   ["precision"]=>
//   int(34)
// }


/**
 * This should NOT reuse $c because we evaluate the RHS before we assign.
 * The refcount of $c is 2 during the operation.
 */
$c = $c->add($c);
debug_zval_dump($c);
// object(Decimal\Decimal)#1 (2) refcount(2){
//   ["value"]=>
//   string(2) "14" refcount(1)
//   ["precision"]=>
//   int(34)
// }



/**
 * This should reuse $value as if it wasn't a reference.
 */
function addto(\Decimal\Decimal &$value, int $n) {
    $value += $n;
}

addto($c, 9);
debug_zval_dump($c);
// object(Decimal\Decimal)#1 (2) refcount(2){
//   ["value"]=>
//   string(2) "23" refcount(1)
//   ["precision"]=>
//   int(34)


/**
 * This should NOT reuse $c because we evaluate the RHS before we assign.
 * The increment is rewritten as `$value = $value + 1`.
 */
function incr(\Decimal\Decimal &$value) {
    $value++;
}

incr($c);
debug_zval_dump($c);
// object(Decimal\Decimal)#2 (2) refcount(2){
//   ["value"]=>
//   string(2) "24" refcount(1)
//   ["precision"]=>
//   int(34)
// }


/**
 * This should NOT reuse anything because we evaluate the RHS before we assign.
 */
function mulby(\Decimal\Decimal &$value, int $n) {
    $value = $value->mul($n);
}

mulby($c, 2);
debug_zval_dump($c);
// object(Decimal\Decimal)#1 (2) refcount(2){
//   ["value"]=>
//   string(2) "48" refcount(1)
//   ["precision"]=>
//   int(34)
// }


/**
 * Test what happens when an exception is thrown when attempting to share.
 */
try {
    $c += "a";
} catch (\Throwable $e) {
    printf("%s: %s\n", get_class($e), $e->getMessage());
}
debug_zval_dump($c);


?>
--EXPECTF--
object(Decimal\Decimal)#%d (2) refcount(2){
  ["value"]=>
  string(1) "3" refcount(1)
  ["precision"]=>
  int(5)
}
object(Decimal\Decimal)#%d (2) refcount(2){
  ["value"]=>
  string(1) "6" refcount(1)
  ["precision"]=>
  int(5)
}
object(Decimal\Decimal)#%d (2) refcount(2){
  ["value"]=>
  string(1) "7" refcount(1)
  ["precision"]=>
  int(5)
}
object(Decimal\Decimal)#%d (2) refcount(2){
  ["value"]=>
  string(2) "14" refcount(1)
  ["precision"]=>
  int(5)
}
object(Decimal\Decimal)#%d (2) refcount(2){
  ["value"]=>
  string(2) "23" refcount(1)
  ["precision"]=>
  int(5)
}
object(Decimal\Decimal)#%d (2) refcount(2){
  ["value"]=>
  string(2) "24" refcount(1)
  ["precision"]=>
  int(5)
}
object(Decimal\Decimal)#%d (2) refcount(2){
  ["value"]=>
  string(2) "48" refcount(1)
  ["precision"]=>
  int(5)
}
DomainException: Failed to parse string as decimal: "a"
object(Decimal\Decimal)#%d (2) refcount(2){
  ["value"]=>
  string(3) "NAN" refcount(1)
  ["precision"]=>
  int(5)
}
