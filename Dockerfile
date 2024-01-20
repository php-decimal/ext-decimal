FROM php:8.3-alpine

RUN apk add --no-cache -X http://dl-cdn.alpinelinux.org/alpine/edge/main mpdecimal-dev

RUN apk add --no-cache $PHPIZE_DEPS valgrind

COPY --from=composer:latest /usr/bin/composer /usr/local/bin/composer