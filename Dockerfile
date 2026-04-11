FROM php:8.2-cli

RUN apt-get update && apt-get install -y \
    libmpdec-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /ext-decimal

COPY . .

RUN phpize \
    && ./configure \
    && make -j$(nproc) \
    && make install

CMD ["make", "test", "TESTS=tests", "NO_INTERACTION=1"]