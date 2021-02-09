FROM php:7.4-cli-buster AS build

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    wget wget unzip libmpdec-dev

RUN wget https://github.com/zlodes/ext-decimal/archive/1.x-php8.zip -O /ext-decimal.zip && \
    unzip /ext-decimal.zip -d /

WORKDIR /ext-decimal-1.x-php8

ARG environment=production
RUN cp /usr/local/etc/php/php.ini-${environment} /usr/local/etc/php/php.ini

RUN phpize
RUN ./configure
RUN make

ENV NO_INTERACTION=1

RUN make test