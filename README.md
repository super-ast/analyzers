# Compiling an analyzer
  1. Clone this repository: `git clone https://github.com/super-ast/analyzers.git`
  2. Change the current working directory to the repository location: `cd analyzers`
  3. Initialize and download the repository submodules: `git submodule update --init --recursive`
  4. Create a `build` directory to keep things clean: `mkdir build`
  5. Change the current working directory to the `build` directory: `cd build`
  6. Run `cmake ..` to generate a `Makefile` of the analyzers.
  7. Run `make [analyzer]` where `[analyzer]` is the analyzer name to generate its executable.
     Run `make` or `make all` to build all the analyzers.

# Current analyzers
## grapher
Reads a `super_ast` in `JSON` format and prints a graph in `dot` format that represents the `super_ast`.

```bash
./grapher < examples/fibonacci.json | dot -Tps -o fibonacci.ps
```

## printer
Reads a `super_ast` in `JSON` format and prints the structure of the resulting `super_ast`.

### Examples
#### Hello world
The input can be found in `lib/base/examples/hello_world.json`.

```
0    BLOCK
1      FUNCTION int main
3        BLOCK
4          FUNCTION_CALL print
5            +
6              Hello world!
7              \n
8          RETURN
9            INT(0)
```

#### Fibonacci
The input can be found in `lib/base/examples/fibonacci.json`.

```
0    BLOCK
1      FUNCTION int fibonacci
3        VAR int n
5        BLOCK
6          CONDITIONAL
7            <=
8              ID(n)
9              INT(1)
10           BLOCK
11             RETURN
12               ID(n)
13         RETURN
14           +
15             FUNCTION_CALL fibonacci
16               -
17                 ID(n)
18                 INT(1)
19             FUNCTION_CALL fibonacci
20               -
21                 ID(n)
22                 INT(2)
```

#### Structs
The input can be found in `lib/base/examples/structs.json`.

```
0    BLOCK
1      STRUCT User
2        VAR string name
4        VAR string password
6      VAR struct[User] user
8      =
9        []
10         ID(user)
11         name
12       Rene
13     =
14       []
15         ID(user)
16         password
17       ithinkthereforeiexist
```

## check
Reads a `super_ast` in `JSON` format and performs validations specified as arguments.
Run `./check --help` for more information.
