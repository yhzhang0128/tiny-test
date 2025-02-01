main: checkdisk.c file0.c ramdisk.c main.c
	gcc -o main -I. -DHW_FS -DMKFS main.c checkdisk.c file0.c ramdisk.c

clean:
	rm -f main