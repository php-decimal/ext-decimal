<?php
use Decimal\Decimal;
use Decimal\Rational;

ini_set("memory_limit", -1);

/* Number of times to perform each calculation. */
$iterations = 100000;

/* Candidates */
define("DECIMAL",  "decimal");
define("RATIONAL", "rational");
define("BC_MATH",  "bcmath");

/* Values used for each benchmark */
$samples = [
    "int" => [
        1,
        2,
        7,
        13,
        57,

        123456789,
        987654321,

       -123456789,
       -987654321,

        PHP_INT_MAX,
        PHP_INT_MIN,
    ],

    "string" => [
        "0.1",
        "0.0000123456789",

        "12345.6789",
       "-98765.4321",

        "9.87654321",
       "-1.23456789",

        "0.00000000123456789",
       "-0.00000000123456789",

        "12345.6789",
       "-12345.6789",
    ],
];

/* Calculates the runtime of each candidate, prints progress, updates report. */
function benchmark(string $action, array $calculations)
{
    global $samples;
    global $iterations;

    echo "\n\t$action";
    echo "\n\t" . str_repeat("-", strlen($action));

    foreach ($samples as $type => $values) {

        /* Each candidate has a calculation function. */
        foreach ($calculations as $name => $calculation) {
            $name = str_pad($name, 10, " ");
            $type = str_pad($type, 10, " ");

            echo "\n\t\t$name\ttype: $type\t";

            $start = microtime(true);

            for ($i = 0; $i < $iterations; $i++) {
                $calculation($values);
            }

            $runtime = microtime(true) - $start;
            $result  = $calculation($values);

            /* Display results as we go... */
            echo "time: " . number_format($runtime, 4) . "\t";
            echo "result: " . $result;
        }

        echo "\n";
    }
}

/**
 * ADD
 */
benchmark("add", [
    BC_MATH => function(array $values): string {
        $result = "0";
        foreach ($values as $value) {
            $result = bcadd($result, $value, 50);
        }
        return $result;
    },

    DECIMAL => function(array $values): string {
        $result = Decimal::valueOf("0", 100);
        foreach ($values as $value) {
            $result += $value;
        }
        return (string) $result;
    },

    RATIONAL => function(array $values): string {
        $result = Rational::valueOf("0");
        foreach ($values as $value) {
            $result += $value;
        }
        return (string) $result;
    },
]);

// /**
//  * SUBTRACT
//  */
// benchmark("sub", [
//     // BC_MATH => function(array $values): string {
//     //     $result = "0";
//     //     foreach ($values as $value) {
//     //         $result = bcsub($result, $value, 50);
//     //     }
//     //     return $result;
//     // },

//     // DECIMAL => function(array $values): string {
//     //     $result = Decimal::valueOf("0", 100);
//     //     foreach ($values as $value) {
//     //         $result = $result - $value;
//     //     }
//     //     return (string) $result;
//     // },

//     RATIONAL => function(array $values): string {
//         $result = Rational::valueOf("0");
//         foreach ($values as $value) {
//             $result = $result - $value;
//         }
//         return (string) $result;
//     },
// ]);

// /**
//  * MULTIPLY
//  */
// benchmark("mul", [
//     // BC_MATH => function(array $values): string {
//     //     $result = "1";
//     //     foreach ($values as $value) {
//     //         $result = bcmul($result, $value, 50);
//     //     }
//     //     return $result;
//     // },

//     // DECIMAL => function(array $values): string {
//     //     $result = Decimal::valueOf("1", 100);
//     //     foreach ($values as $value) {
//     //         $result = $result * $value;
//     //     }
//     //     return (string) $result;
//     // },

//     RATIONAL => function(array $values): string {
//         $result = Rational::valueOf("1");
//         foreach ($values as $value) {
//             $result = $result * $value;
//         }
//         return (string) $result;
//     },
// ]);

/**
 * DIVIDE
 */
benchmark("div", [
    BC_MATH => function(array $values): string {
        $result = '1';
        foreach ($values as $value) {
            $result = @bcdiv($result, $value, 50);
        }
        return (string) $result;
    },

    DECIMAL => function(array $values): string {
        $result = Decimal::valueOf('1', 100);
        foreach ($values as $value) {
            $result /= $value;
        }
        return (string) $result;
    },

    RATIONAL => function(array $values): string {
        $result = Rational::valueOf("1");
        foreach ($values as $value) {
            $result /= $value;
        }
        return (string) $result;
    },
]);
