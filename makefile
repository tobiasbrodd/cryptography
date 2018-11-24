CC = g++
FLAGS = -std=c++17 -g -Wall -Wextra -pedantic -fsanitize=undefined,address

bacon: bacon_cipher/bacon.cpp parser.cpp
	$(CC) $(FLAGS) bacon_cipher/bacon.cpp parser.cpp -o bacon.out

caesar: caesar_cipher/caesar.cpp parser.cpp
	$(CC) $(FLAGS) caesar_cipher/caesar.cpp parser.cpp -o caesar.out

rsa: rsa_cipher/rsa.cpp parser.cpp
	$(CC) $(FLAGS) rsa_cipher/rsa.cpp parser.cpp -o rsa.out

clean: 
	rm -rf *.dSYM
	rm *.o *.out
