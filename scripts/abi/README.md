# Application Binary Interface (ABI)

This directory contains some scripts to generate a JSON interface that describes
the AFF3CT C++ headers. This JSON interface can facilitate the creation of
AFF3CT wrappers for high level languages (Python, MATLAB, Julia, ...) that need
the AFF3CT headers information to be generated.

## Dump the Abstract Syntax Tree (AST)

The first operation consists in dumping the AFF3CT AST. To do this, we rely on
the [Clang](https://clang.llvm.org/) compiler.

Install Clang on Ubuntu 20.04 LTS:

```bash
sudo apt install clang
```

Then you can generate the AST:

```bash
./01_aff3ct_dump_clang_ast.sh
```

This last command should generate an `aff3ct_ast.txt` file in the current
directory.

## Convert the AST in JSON

In order to facilitate the manipulation of the AFF3CT AST, we propose to
generate a more structured file from the previously generated `aff3ct_ast.txt`
file. We choose to use the JSON format as it is highly democratized.

The conversion script is written in [Python](https://www.python.org/) version 3,
so on Ubuntu 20.04 LTS you should install it as follow :

```bash
sudo apt install python3
```

Finally you can convert the `aff3ct_ast.txt` raw AST file into the JSON format:

```bash
./02_aff3ct_read_clang_ast.py
```

This last command should generate a `db.json` file in the current directory.
