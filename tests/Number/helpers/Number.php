<?php

/**
 *
 */
class Number extends \Decimal\Number
{
    protected $value;

    protected function __construct($value)
    {
        $this->value = $value;
    }

    public static function valueOf($value): \Decimal\Number
    {
        return new static($value);
    }

    private function parse($other)
    {
        if ($other instanceof Number) {
            $other = $other->value;
        }

        return $other;
    }

    public function add($other): \Decimal\Number
    {
        printf("%s\n", __METHOD__);

        return new static($this->value + $this->parse($other));
    }

    public function sub($other): \Decimal\Number
    {
        printf("%s\n", __METHOD__);

        return new static($this->value - $this->parse($other));
    }

    public function mul($other): \Decimal\Number
    {
        printf("%s\n", __METHOD__);

        return new static($this->value * $this->parse($other));
    }

    public function div($other): \Decimal\Number
    {
        printf("%s\n", __METHOD__);

        return new static($this->value / $this->parse($other));
    }

    public function pow($other): \Decimal\Number
    {
        printf("%s\n", __METHOD__);

        return new static($this->value ** $this->parse($other));
    }

    public function mod($other): \Decimal\Number
    {
        printf("%s\n", __METHOD__);

        return new static($this->value % $this->parse($other));
    }

    public function shiftl($places): \Decimal\Number
    {
        printf("%s\n", __METHOD__);

        return new static($this->value * (10 ** $places));
    }

    public function shiftr($places): \Decimal\Number
    {
        printf("%s\n", __METHOD__);

        return new static($this->value / (10 ** $places));
    }

    public function floor(): \Decimal\Number
    {
        // No need to implement this.
    }

    public function ceil(): \Decimal\Number
    {
        // No need to implement this.
    }

    public function trunc(): \Decimal\Number
    {
        // No need to implement this.
    }

    public function round(?int $places = NULL, ?int $mode = NULL): \Decimal\Number
    {
        // No need to implement this.
    }

    public function abs(): \Decimal\Number
    {
        // No need to implement this.
    }

    public function negate(): \Decimal\Number
    {
        // No need to implement this.
    }

    public function parity(): int
    {
        // No need to implement this.
    }

    public function signum(): int
    {
        // No need to implement this.
    }

    public function isNaN(): bool
    {
        return (string) $this->value === "NAN";
    }

    public function isInf(): bool
    {
        // No need to implement this.
    }

    public function isInteger(): bool
    {
        // No need to implement this.
    }

    public function isZero(): bool
    {
        // No need to implement this.
    }

    public function toFixed(?int $places = NULL, ?bool $commas = NULL, ?int $mode = NULL): string
    {
        // No need to implement this.
    }

    public function toString(): string
    {
        printf("%s\n", __METHOD__);

        return (string) $this->value;
    }

    public function toInt(): int
    {
        printf("%s\n", __METHOD__);

        return (int) $this->value;
    }

    public function toFloat(): float
    {
        printf("%s\n", __METHOD__);

        return (float) $this->value;
    }

    public function toDecimal(int $precision): \Decimal\Decimal
    {
        printf("%s\n", __METHOD__);

        return \Decimal\Decimal::valueOf($this->value, $precision);
    }

    public function toRational(): \Decimal\Rational
    {
        printf("%s\n", __METHOD__);

        return \Decimal\Rational::valueOf($this->value);
    }

    public function compareTo($other): int
    {
        printf("%s\n", __METHOD__);

        return $this->value <=> $this->parse($other);
    }
}
