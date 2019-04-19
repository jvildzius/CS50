# Questions

## What's `stdint.h`?

This header from the C library helps declare sets of integer types having specified number of bits to store their value (width). Typedefs are then used to specify the bounds of this.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

This specifies an exact width (as mentioned above) for each data type.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

BYTE: 1 byte.
DWORD: 4 bytes.
LONG: 4 bytes.
WORD: 2 bytes.

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

66 77 in ASCII.

## What's the difference between `bfSize` and `biSize`?

bfSize is the size, in bytes, of the bitmap file.
biSize is the number of bytes required by the structure.

## What does it mean if `biHeight` is negative?

If biHeight is negative, the bitmap is a top-down DIB and its origin is the upper-left corner.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

biBitCount does this.

## Why might `fopen` return `NULL` in lines 24 and 32 of `copy.c`?

If there is not enough memory to do so.

## Why is the third argument to `fread` always `1` in our code?

I'm not sure if it is beacause we want to iterate through 1 block or pixel at a time,
or if, in lines 40 & 44 of copy.c, it is to take in the File and Info headers each in 1 "go"
(because it is doing the size of the entire header one time)

## What value does line 63 of `copy.c` assign to `padding` if `bi.biWidth` is `3`?

(4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4
bi.biWidth is 3 (pixels) and RGBTRIPLE is 3 bytes.
If it is compatible to multiple these, then it ultimately assigns 3.

## What does `fseek` do?

It allows us to skip over any padding by moving the file pointer a certain amount and "picking up where we left off" on the bytes of the image.

## What is `SEEK_CUR`?

When moving a file pointer to somewhere, SEEK_CUR indicates that it should be moved "from" the current location.