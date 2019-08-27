--TEST--
Decimal::pow
--FILE--
<?php
use Decimal\Decimal;

/**
 * op1, op2, expected result, expected precision
 */
$tests = [
    [   
        Decimal::valueOf(1) / Decimal::valueOf(3),     
        "2.0", 
        "0.1111111111111111111111111111111111",                      
        Decimal::DEFAULT_PRECISION
    ],
    [   
        Decimal::valueOf(1) / Decimal::valueOf(3),     
        "1.5", 
        "0.1924500897298752548363829268339858",                      
        Decimal::DEFAULT_PRECISION
    ],
    [   
        Decimal::valueOf(1) / Decimal::valueOf(3),     
        "100", 
        "0.000000000000000000000000000000000000000000000001940325217482632837588506028804631",                   
        Decimal::DEFAULT_PRECISION
    ],
    [   
        Decimal::valueOf(1, 50) / Decimal::valueOf(3, 50), 
        "100", 
        "0.0000000000000000000000000000000000000000000000019403252174826328375885060288046503812141166864962",   
        50
    ],
    [
        Decimal::valueOf("0.7"), 
        "1000", 
        "0.00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001253256639965718318107554832382734", 
        Decimal::DEFAULT_PRECISION
    ],

    [Decimal::valueOf("0"),   "0",    pow(0, 0), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("0"),   "1",    pow(0, 1), Decimal::DEFAULT_PRECISION],

    [Decimal::valueOf("1"),  "-2",    pow(1, -2), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("1"),  "-1",    pow(1, -1), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("1"),   "0",    pow(1,  0), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("1"),   "1",    pow(1,  1), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("1"),   "2",    pow(1,  2), Decimal::DEFAULT_PRECISION],

    [Decimal::valueOf("-1"), "-2",    pow(-1, -2), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-1"), "-1",    pow(-1, -1), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-1"),  "0",    pow(-1,  0), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-1"),  "1",    pow(-1,  1), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-1"),  "2",    pow(-1,  2), Decimal::DEFAULT_PRECISION],

    /* Test pow by self */
    [$a = Decimal::valueOf("1.2345678"), $a, "1.297123731240147537684763556982358", Decimal::DEFAULT_PRECISION],

    /* Test that default precision is assumed for scalar operands. */
    [Decimal::valueOf("1", 10), "1", "1", 10],
    [Decimal::valueOf("1", 50), "1", "1", 50],
];

foreach ($tests as $test) {
    list($op1, $op2, $expect, $prec) = $test;

    $results = [
        $op1->pow($op2),
        $op1 ** $op2,
    ];

    foreach ($results as $result) {
        if ((string) $result !== (string) $expect || $result->precision() !== $prec) {
            print_r(compact("op1", "op2", "result", "expect", "prec"));
            break;
        }
    }
}
?>
--EXPECTF--
