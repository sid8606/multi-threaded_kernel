x86_64 
sudo apt install qemu-system-x86
sudo apt install nasm
sudo apt install bless

1. build
nasm -f bin ./boot.asm -o boot.bin

2. Disassemble
ndisasm boot.bi

3. Run
qemu-system-x86_64 -hda ./boot.bin

Protected  Mode Development

target remote | qemu-system-x86_64 -hda ./boot.bin -S -gdb stdio
layout asm --> dumps assembly code
info registers	 --> prints registers values


Add kernel symbol table into GDB

add-symbol-file ./build/kernelfull.o 0x0100000

target remote | qemu-system-x86_64 -hda ./bin/os.bin -S -gdb stdio
