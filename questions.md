# Questions

## What's `stdint.h`?

library that lets you define your own types

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

specify what kind of data types you're going to be using. let's you alias for easier usage in previously written code

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

Byte - 1
DWORD - 4
LONG - 4
WORD - 2

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

Must be 0x4d42 (BM flipped)

## What's the difference between `bfSize` and `biSize`?

bfSize is the number of bytes of the file, while biSize is the number of bytes required for that particular structure

## What does it mean if `biHeight` is negative?

Means the file is top down and you start reading from top left corner

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

biBitCount

## Why might `fopen` return `NULL` in `copy.c`?

If the file doesn't exist

## Why is the third argument to `fread` always `1` in our code?

Only pulling one element's worth of data

## What value does `copy.c` assign to `padding` if `bi.biWidth` is `3`?

(4 - (3*24)%4) % 4 = (4 - 72%4) % 4 = 4 - 0 % 4 = 0

## What does `fseek` do?

Offsets the current reading position of the file by the digits in the padding

## What is `SEEK_CUR`?

Current location of where to start from
