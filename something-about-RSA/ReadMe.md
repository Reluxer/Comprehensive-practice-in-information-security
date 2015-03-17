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

- Then, get the [package of GMP library](GMP/gmp-6.0.0a.tar.bz2). If you come into contact for the first time with this library, [here](GMP/GMP_Chinese_Introduction_3.pdf) is a Chinese Introduction book about it.You can download it and read.
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


- [RSA_Attack_01.c](RSA_Attack_01.c) Achieve the attack of RSA algorithm. Need a couple support files, tableCipherText1 and tableCipherText2.

![Effect diagram](img/2.png)

### 2
Assume:
	C<sub>1</sub> = m<sup>e</sup> mod n<sub>1</sub>,
	C<sub>2</sub> = m<sup>e</sup> mod n<sub>2</sub>,
	...,
	C<sub>t</sub> = m<sup>e</sup> mod n<sub>t</sub>,

And `t>e`.That we can get the value of m<sup>e</sup> mod(n<sub>1</sub>n<sub>2</sub>...n<sub>t</sub>) by Chinese remainder theorem.The details are as follows:

m<sup>e</sup>≡∑N<sub>i</sub>N<sub>i</sub><sup>-1</sup>C<sub>i</sub> mod N(i:=1...t)  
N=n<sub>1</sub>n<sub>2</sub>...n<sub>t</sub>  
N<sub>i</sub>=N/n<sub>i</sub>  
N<sub>i</sub>N<sub>i</sub><sup>-1</sup>≡1 mod n<sub>i</sub>

Because of `t>e && m<n` it equals m<sup>e</sup>, then we can get m easily.

Include this files:

[RSAFor02.c](RSAFor02.c) generate a set of C<sub>t</sub>, By using redirection, origin data is stored in file [result](result).

[table_C](table_C),[table_D](table_D),[table_N](table_N),[table_E](table_E),[table_M](table_M) Record the value of C,D, N, E,M

[RSA_Attack_02.c](RSA_Attack_02.c) Details for attacking process.

![Effect diagram](img/3.png)