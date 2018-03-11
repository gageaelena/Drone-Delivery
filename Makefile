all: build

build: livrare

minimax: livrare.c
		gcc -Wall livrare.c -o livrare

clean:
		rm -rf livrare
