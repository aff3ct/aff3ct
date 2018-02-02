# AFF3CT Debug Parser
This tool is used to parse the debug outputs of the [AFF3CT Software](https://aff3ct.github.io) and convert it into binary and text files.
## Usage
```bash
usage: aff3ct_debug_parser.py [-h] --mod MOD --tsk TSK [-o OUTPUT] [--src SRC]
                              path

positional arguments:
  path                  path to the file to be parsed

optional arguments:
  -h, --help            show this help message and exit
  --mod MOD             module to be extracted, ex : Source_random_fast
  --tsk TSK             task to be extracted, ex : generate
  -o OUTPUT, --output OUTPUT
                        path to the output folder
  --src SRC             export specified frame as .h source file
```
## Test and example
In the project root directory, run : 

	./test/test.sh

## Output files format
### Text (file.txt)
```
	8   // Number of Frames
	int8 // Format in {int8, int16, int32, int64, float32, float64}
	32   // Frame Length
	-128 0 -128 0 0 -128 0 -128 -128 -128 -128 0 -128 0 0 0 -128 -128 0 -128 -128 0 0 0 -128 -128 0 0 0 -128 0 -128
	0 -128 0 -128 0 0 -128 0 0 0 0 0 -128 0 -128 -128 0 -128 -128 0 -128 0 -128 0 0 0 0 -128 0 0 0 -128
	0 0 -128 -128 -128 0 0 -128 0 -128 0 0 -128 0 -128 0 -128 -128 -128 0 -128 -128 -128 0 -128 0 0 0 0 -128 0 0
	0 -128 -128 -128 -128 -128 -128 -128 0 0 -128 -128 -128 0 -128 0 0 -128 -128 -128 -128 -128 -128 0 0 -128 0 0 -128 -128 0 0
	0 -128 0 0 -128 0 0 -128 0 -128 0 -128 0 -128 -128 0 0 0 0 -128 0 -128 0 0 0 0 0 -128 0 0 0 0
	0 -128 -128 0 0 -128 0 -128 0 0 0 -128 0 -128 0 0 -128 -128 0 0 0 0 -128 0 -128 -128 -128 0 -128 -128 -128 0
	-128 0 0 0 0 -128 0 0 -128 -128 -128 -128 -128 0 0 0 -128 0 -128 0 -128 0 -128 0 -128 0 -128 0 0 0 0 0
	0 0 0 -128 0 0 0 -128 0 0 -128 0 -128 -128 0 -128 0 0 -128 -128 0 -128 0 0 0 0 -128 0 0 0 0 -128
```
### Binary (file.bin)
Every value is in big endian format.
First 32 bits : Number Of Frames
Second 32 bits : if 0, integer values, if 1, float values
Third 32 bits : Number of bits per value
Fourth 32 bits : Frame length
These are followed by (Number of frames x Frame length) values of the number of bits per value, no separator
```
0000 0008 0000 0000 0000 0008 0000 0020
8000 8000 0080 0080 8080 8000 8000 0000
8080 0080 8000 0000 8080 0000 0080 0080
0080 0080 0000 8000 0000 0000 8000 8080
0080 8000 8000 8000 0000 0080 0000 0080
0000 8080 8000 0080 0080 0000 8000 8000
8080 8000 8080 8000 8000 0000 0080 0000
0080 8080 8080 8080 0000 8080 8000 8000
0080 8080 8080 8000 0080 0000 8080 0000
0080 0000 8000 0080 0080 0080 0080 8000
0000 0080 0080 0000 0000 0080 0000 0000
0080 8000 0080 0080 0000 0080 0080 0000
8080 0000 0000 8000 8080 8000 8080 8000
8000 0000 0080 0000 8080 8080 8000 0000
8000 8000 8000 8000 8000 8000 0000 0000
0000 0080 0000 0080 0000 8000 8080 0080
0000 8080 0080 0000 0000 8000 0000 0080
```

### Source (file.h)
One frame can be exported as a source file with `--src` argument
Example :
```c
#ifndef V_K_H
#define V_K_H

signed char V_K[32] = {
	   0, -128,    0, -128,    0,    0, -128,    0,
	   0,    0,    0,    0, -128,    0, -128, -128,
	   0, -128, -128,    0, -128,    0, -128,    0,
	   0,    0,    0, -128,    0,    0,    0, -128
};

#endif //V_K_H
```
