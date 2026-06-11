CC = gcc
CFLAGS = -I inc -Wall
SRC = src/stego.c src/encode.c src/decode.c
OUT = stego

all:
	$(CC) $(SRC) $(CFLAGS) -o $(OUT)

encode:
	./stego -e test/input.bmp test/secret.txt test/output.bmp

decode:
	./stego -d test/output.bmp

clean:
	rm -f stego test/output.bmp
