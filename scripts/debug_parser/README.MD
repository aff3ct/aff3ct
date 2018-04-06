# AFF3CT Debug Parser
This tool is used to parse the debug outputs of the [AFF3CT Software](https://aff3ct.github.io) and convert it into binary and text files.
This  debug parser supports the hex debug format (`--sim-debug-hex`), which is highly recommended when dealing with floating point representation.
## Usage
```bash
usage: aff3ct_debug_parser.py [-h] --mod MOD --tsk TSK [--txt] [--bin] [--src]
                              [--fra FRA] [-o OUTPUT]
                              path

positional arguments:
  path                  path to the file to be parsed

optional arguments:
  -h, --help            show this help message and exit
  --mod MOD             module to be extracted, ex : Source_random_fast
  --tsk TSK             task to be extracted, ex : generate
  --txt                 export as txt
  --bin                 export as bin
  --src                 export as c source
  --fra FRA             export a single frame, whose index is specified
  -o OUTPUT, --output OUTPUT
                        path to the output folder
```
## Test and examples
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


## Tips
To avoid big temporary files when dumping a lof of frames, you can pipe the AFF3CT command with a `grep` parser.
For example, when you want to get only frames from the LDPC encoder, you can do:
```bash
./bin/aff3ct -C "LDPC" ... --sim-debug | grep -A 3 Encoder | grep -v '\-\-' > debug
../scripts/debug_parser/aff3ct_debug_parser.py --mod Encoder_LDPC --tsk encode --txt debug
```

The `grep -A 3` is to return the 3 following lines (the frames themselves) in more than the line with "Encoder".
The `grep -v '\-\-'` is to remove unexpected lines added by the above `grep`.

The pipe is working dynamically, it does not use RAM.
However, you can still unbuffer the pipes if you want with the [unbuffer command](https://unix.stackexchange.com/questions/25372/turn-off-buffering-in-pipe?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa)


## Future work
Need to pipe directly the AFF3CT output on the debug parser by certainly using [sys.stdin](https://stackoverflow.com/questions/1450393/how-do-you-read-from-stdin-in-python?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa)
