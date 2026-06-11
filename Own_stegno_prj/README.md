To compile
gcc src/stego.c src/encode.c src/decode.c -I inc -o stego
or
make

To run
./stego -e test/input.bmp test/secret.txt test/output.bmp
./stego -d test/output.bmp
or
make encode
make decode

To clean:
make clean

NOTE:
make clean deletes generated / intermediate files, such as:
executable → stego
object files → *.o
temporary build outputs

It does NOT delete:
source files (.c, .h)
input files (input.bmp, secret.txt)
output files unless you explicitly add them
