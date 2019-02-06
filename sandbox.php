<?php

use Decimal\Decimal;

/**
 * Decimal objects are immutable. Any arithmetic op produces a new decimal as
 * the result. This is wasteful in cases where intermediates are not necessary.
 */


/**
 * This is how it worked before the optimisation.
 *
 * 1. Create a new decimal with value 1
 * 2. Add integer 2, and write that to a new decimal instance.
 * 3. Assign to $a
 */
$a = Decimal::valueOf(1)->add(2);
printf("---\n");

/**
 * This is how it works with the optimisation in place.
 *
 * 1. Create a new decimal with value 1
 * 2. Add integer 2, using the decimal object created in 1 for the result.
 * 3. Assign to $a
 */
$a = Decimal::valueOf(1)->add(2);
printf("---\n");

/**
 * This was what I wanted to achieve...
 *
 * We shouldn't need to create a new instance when we add, but $a' refcount is 2
 * and I don't think we can re-use it as the result.
 */
$a = Decimal::valueOf(0);
$a = $a->add(1);
$a = $a->add(2);

/**
 * ^ This should only create a single instance, and use $a as the result every
 *   time. But computers are dumb and it is not aware that we are writing to
 *   $a, using $a. This code will create 3 instances of Decimal, when only one
 *   should be necessary.
 *
 *   Is achieving this possible?
 */
