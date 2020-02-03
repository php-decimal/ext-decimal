--TEST--
Number json_encode
--FILE--
<?php
require __DIR__ . "/helpers/Number.php";

var_dump(json_encode(Number::valueOf("0.3")));

/**
 * Test that we can override the internal alias.
 */
class Override extends Number
{
    public function jsonSerialize(): string
    {
        return "*";
    }
}

var_dump(json_encode(Override::valueOf(5)));

?>
--EXPECT--
string(5) ""0.3""
string(3) ""*""