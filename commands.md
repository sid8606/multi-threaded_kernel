x86_64 
sudo apt install qemu-system-x86
sudo apt install nasm
sudo apt install bless

Build cross-gcc
https://wiki.osdev.org/GCC_Cross-Compiler
You can download the desired Binutils release by visiting the Binutils website or directly accessing the GNU main mirror.
https://ftp.gnu.org/gnu/binutils/
You can download the desired GCC release by visiting the GCC website or directly accessing the GNU main mirror.
https://www.gnu.org/software/gcc/mirrors.html

#Preparation
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

#Binutils
cd $HOME/src

mkdir build-binutils
cd build-binutils
../binutils-x.y.z/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install

#GCC

cd $HOME/src

# The $PREFIX/bin dir _must_ be in the PATH. We did that above.
which -- $TARGET-as || echo $TARGET-as is not in the PATH

mkdir build-gcc
cd build-gcc
../gcc-x.y.z/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc

Using the new Compiler
$HOME/opt/cross/bin/$TARGET-gcc --version
export PATH="$HOME/opt/cross/bin:$PATH"


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
target remote | qemu-system-i386 -hda ./bin/os.bin -S -gdb stdio

