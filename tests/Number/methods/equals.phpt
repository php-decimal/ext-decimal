--TEST--
Number::equals
--FILE--
<?php
require __DIR__ . "/../helpers/Number.php";

use \Decimal\Decimal;
use \Decimal\Rational;

$tests = [

    // op1, op2, expected
    [
        [Number::valueOf(4),    "4",    true],
        [Number::valueOf(4),    "4.0",  true],
        [Number::valueOf(4),     4,     true],
        [Number::valueOf(4),     4.0,   true],
    ],
    [
        [Number::valueOf(4),    "5",    false],
        [Number::valueOf(4),    "4.5",  false],
        [Number::valueOf(4),     5,     false],
        [Number::valueOf(4),     4.5,   false],
    ],
    [
        [Number::valueOf(4),     Number::valueOf(4),   true],
        [Number::valueOf(4),     Number::valueOf(5),   false],
    ],
    [
        [Number::valueOf(NAN),   true,                  true],
        [Number::valueOf(NAN),   NAN,                   false],
        [Number::valueOf(NAN),   Number::valueOf(0),    false],
        [Number::valueOf(NAN),   Number::valueOf(NAN),  false],
    ]
];

$index = 0;

foreach ($tests as $group) {
    foreach ($group as $test) {
        $index++;

        list($op1, $op2, $expect) = $test;

        $results = [
            $op1 == $op2,
            $op2 == $op1,
            $op1->equals($op2),
        ];

        foreach ($results as $result) {
            if ($result !== $expect) {
                print_r(compact("index", "op1", "op2", "result", "expect"));
                break;
            }
        }
    }
    
    printf("\n");
}

?>
--EXPECT--
Number::compareTo
Number::compareTo
Number::compareTo
Number::compareTo
Number::compareTo
Number::compareTo