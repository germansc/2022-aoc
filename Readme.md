# Advent of Code 2022

Repo for versioning solutions for the Advent of Code 2022 Challenges.

Each Challenge is solved under a `dayXX` directory with the following
structure:

```
dayXX
├── build
│   └── Makefile
├── Challenge.md
├── Input.txt
└── src
    └── main.c
```
* **build** contains the Makefile for compiling the solution.
* **src** contains the solution code.
* **Challenge.md** has the body of the challenge.
* **Input.txt** is the input I was given.

### Compiling the solutions

All programs are written to take input from `stdin`. To compile and run an
example the steps would be:

```
cd dayXX/build
make
./dayXX < ../Input.txt
```

