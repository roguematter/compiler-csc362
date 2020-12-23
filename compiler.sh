./compiler $1 > ../output/output.asm
nasm -f elf -l ../output/output.lst ../output/output.asm
ld -o ../output/output ../output/output.o
