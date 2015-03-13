# Implementation and attack of RSA algorithm based on GMP library

## Desciption

This experiment need to do these things:

- Install the GMP library
- Implementation of RSA algorithm
- Achive the attack of RSA algorithm

## Environment

Ubuntu 14.04 LTS 32bit

## Install

- In order to make the program work with no panic,you need to install g++, gcc, m4 firstly. Like this:

		sudo apt-get install g++ gcc m4

- Then, get the [package of GMP library](GMP/gmp-6.0.0a.tar.bz2)
- Next step, install the GMP library, do like this:

    	tar jxvf gmp-6.0.0a.tar.bz2
    	cd gmp-6.0.0
		./configure
    	make
		make check
		make install

- OK, that is enough.
- If you want to compile the source code file and run it, do:

		gcc -W example.c -o example -lgmp
    	./example

## Attack methods

### 1

Assume the message to be encrypted can be divided into two digital m1, m2. That is to say, message = m1 * m2.

If the message can reache the maximum value of `2^length`, we can enumeration values from 1 to `2^(length/2)`.

Include this files:

- [RSA.c](RSA.c) Implementation of RSA algorithm.


![Effect diagram](img/1.png)

- [RSA_GenerateCipherTextTable.c](RSA_GenerateCipherTextTable.c) Choose a set of key, generate a couple cipher text table.Run it like this:

	gcc -W RSA_GenerateCipherTextTable.c -o 1 -lgmp 
    ./1 > tableCipherText1
	./1 > tableCipherText2


- [RSA_Attack.c](RSA_Attack.c) Achieve the attack of RSA algorithm. Need a couple support files, tableCipherText1 and tableCipherText2.

![Effect diagram](img/2.png)

### 2
Assume C~1~ = m^e^ mod n~1~, C~2~=m^e^ mod n~2~, ..., C~t~=m^e^ mod n~t~,



