#Ryan Wandke

all: boot kernel shell  run 	 

rdy: boot kernel

shell: lib.o shell.o
	ld86 -o shelll -d shell.o lib.o


shell.o: shell.c boot
	bcc -ansi -c -o shell.o shell.c

lib.o: lib.asm
	as86 lib.asm -o lib.o

boot: bootload.asm
	nasm bootload.asm
	dd if=/dev/zero of=floppya.img bs=512 count=2880
	dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc
	dd if=message.txt of=floppya.img bs=512 count=1 seek=30 conv=notrunc
	dd if=map.img of=floppya.img bs=512 count=1 seek=1 conv=notrunc
	dd if=dir.img of=floppya.img bs=512 count=1 seek=2 conv=notrunc

message: boot kernel 
	gcc -o loadFile loadFile.c
	./loadFile message.txt


bigmess: boot kernel
	./loadFile bigmess.txt

kernel: kernel.o kernel_asm.o
	ld86 -o kernel -d kernel.o kernel_asm.o

kernel.o: kernel.c 
	bcc -ansi -c -o kernel.o kernel.c

kernel_asm.o: kernel.asm
	as86 kernel.asm -o kernel_asm.o
	
run: kernel shell bigmess message
	./loadFile shelll
	dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3
	export DISPLAY=localhost:0.0
	qemu-system-i386 -fda floppya.img -device isa-debug-exit,iobase=0xf4,iosize=0x04 -boot order=a &
