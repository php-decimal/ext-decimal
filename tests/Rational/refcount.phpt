--TEST--
Rational reference counting
--FILE--
<?php
$a = "1.4";
$b = "2.6";

/**
 *  This should reuse the rational created for A as the result.
 */
$c = \Decimal\Rational::valueOf($a)->add($b)->sub(0);
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
function addto(\Decimal\Rational &$value, $n) {
    $value += $n;
}

addto($c, "9.5");
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
function incr(\Decimal\Rational &$value) {
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
function mulby(\Decimal\Rational &$value, $n) {
    $value = $value->mul($n);
}

mulby($c, "2.5");
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
object(Decimal\Rational)#%d (2) refcount(2){
  ["num"]=>
  string(1) "4" refcount(1)
  ["den"]=>
  string(1) "1" refcount(1)
}
object(Decimal\Rational)#%d (2) refcount(2){
  ["num"]=>
  string(1) "7" refcount(1)
  ["den"]=>
  string(1) "1" refcount(1)
}
object(Decimal\Rational)#%d (2) refcount(2){
  ["num"]=>
  string(1) "8" refcount(1)
  ["den"]=>
  string(1) "1" refcount(1)
}
object(Decimal\Rational)#%d (2) refcount(2){
  ["num"]=>
  string(2) "16" refcount(1)
  ["den"]=>
  string(1) "1" refcount(1)
}
object(Decimal\Rational)#%d (2) refcount(2){
  ["num"]=>
  string(2) "51" refcount(1)
  ["den"]=>
  string(1) "2" refcount(1)
}
object(Decimal\Rational)#%d (2) refcount(2){
  ["num"]=>
  string(2) "53" refcount(1)
  ["den"]=>
  string(1) "2" refcount(1)
}
object(Decimal\Rational)#%d (2) refcount(2){
  ["num"]=>
  string(3) "265" refcount(1)
  ["den"]=>
  string(1) "4" refcount(1)
}
DomainException: Failed to parse string as decimal: "a"
object(Decimal\Rational)#%d (2) refcount(2){
  ["num"]=>
  string(3) "NAN" refcount(1)
  ["den"]=>
  string(1) "1" refcount(1)
}
