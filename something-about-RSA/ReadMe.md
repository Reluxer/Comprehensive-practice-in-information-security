# Implementation and attack of RSA algorithm based GMP library

## Desciption

This experiment need to do these things:

- Install the GMP library
- Implementation of RSA algorithm
- Achive the attack of RSA algorithm

## Environment

Ubuntu 14.04 LTS 32bit

## Install

- In order to work with no panic,you need to install g++, gcc, m4 firstly. Like this:

    sudo apt-get install g++ gcc m4

- Then, get the [package of GMP]()
- Do like this:

	tar jxvf gmp-6.0.0a.tar.bz2
    cd gmp-6.0.0
	./configure
    make
	make check
	make install

- OK, that is enough.
- If you want to compile the source code file, do:

	gcc -W example.c -o example -lgmp
    ./example

##Note for files

[RSA.c](RSA.c) Implementation of RSA algorithm.


![Effect diagram](/img/2015-03-11 22:40:55.png)

[RSA_GenerateCipherTextTable.c](RSA_GenerateCipherTextTable.c) Choose a set of key,generate a cipher text table.Run it like this:

	gcc -W RSA_GenerateCipherTextTable.c -o 1 -lgmp 
    ./1 > cipherTextTable1
	./1 > cipherTextTable2


[RSA_Attack.c](RSA_Attack.c) Achieve the attack of RSA algorithm.

![Effect diagram](/img/2015-03-11 23:02:32.png)


