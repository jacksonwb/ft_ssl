# ft_ssl @ 42
This is a partial recode of the OpenSSL crypography library implemented in C

## Description
The goal of this project is to understund the behaviour of the OpenSSL library and gain an understanding of the fundamentals of cryptography by reimplementing some of its functionality from the ground up.

### Current Functions
`md5`
`sha256`
`sha512`

### Future Functions
`base64`
`des`
`des-ecb`
`des-csc`
`genrsa`
`urandom`
`rsa`
`rasutl`

## Using this binary
Build the binary using `make`

`clang` is required to build

Usage for this binary is as follows:
`ft_ssl commad [command opts] [command args]`

### Hash Functions
`md5`
`sha256`
`sha512`

The hashing functions have the following usage:
`[hash_type] [-pqr] [-s string] [files...]`

All hashing functions can read from `STDIN`, take any number of strings specified as args, or any number of files.
The following flags are supported:
`-p` - read from `STDIN`, echo the input and calculate the hash

`-q` - quiet mode

`-r` - reverse output format

`-s` - find the hash for the string to be given after this flag or in the following argument

Note: this sha512 implementation only supports messages with lengths representable using 64 bit integers (~2.3 Exabytes), instead of the 2^128 bit message length required by the standard.