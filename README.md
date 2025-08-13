# simple-rle

Simple compression and decompression program that is based on [Run-Length Encoding](https://en.wikipedia.org/wiki/Run-length_encoding).

## Building

Simply run `make`. You will get two executables in you `build/` directory: `rle` and `unrle`.

## How to use

### rle

To compress several files, enter the following: 

```bash
./build/rle FILE FILE FILE
```

Note: your output files would be appended with `.rle` extension.

### unrle

To uncompress several files, enter the following:

```bash
./build/rle INFILE.rle OUTFILE
```
Your `INFILE` must end with `.rle` extension. Ensure that you used `rle` before using uncompression utility.

## Tests 

Tests can be run with `diff` utility:

```bash
diff ./test/data.tst ./test/data.rle.tst
```

