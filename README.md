# Tiny file system tests

```shell
> make
gcc -o main -I. -DHW_FS -DMKFS main.c checkdisk.c file0.c ramdisk.c
> ./main
 0:  0  0  0  0  0  0  0  0
 1:  0  0  0  0  0  0  0  0
 2:  0  0  0  0  0  0  0  0
 3:  0  0  0  0  0  0  0  0
 4:  0  0  0  0  0  0  0  0
 5:  0  0  0  0  0  0  0  0
 6:  0  0  0  0  0  0  0  0
 7:  0  0  0  0  0  0  0  0
 8:  0  0  0  0  0  0  0  0
 9:  0  0  0  0  0  0  0  0
10:  0  0  0  0  0  0  0  0
11:  0  0  0  0  0  0  0  0
12:  0  0  0  0  0  0  0  0
13:  0  0  0  0  0  0  0  0
14:  0  0  0  0  0  0  0  0
15:  0  0  0  0  0  0  0  0
W:0:1
...
```

This is a tiny test with a tiny disk of 16 blocks while each block has 32 bytes.
Therefore, there are 16 lines printed above, and each line contains 8 4-byte integers.

`W:0:1` means to write to `(ino=0, offset=1)` with some content, and you could see the content details in `main.c`.
After you run this command, `main` will print out the new disk content, so you can check whether the modifications are expected.
Below is a list of commands that you could try.

* `W:ino:offset`
* `W:ino:offset:content`
* `R:ino:offset`
* `R:ino:offset:expected_content`
* `S:ino:new_size`
* `G:ino`
* `G:ino:expected_size`

Note that `W`, `R`, `S` and `G` correspond to write, read, setsize and getsize.
After you have more confidence with your file system code, you can change `BLOCK_SIZE`, `DISK_SIZE` and `MAX_INODES` to much larger numbers, and design some serious test traces.
For example, `trace.txt` is a simple test trace and you can use it as follows.

```
> ./main < trace.txt
...
```

Note that `trace.txt` contains `expected_content` for read operations, so `main` would print error messages if your file system returns unexpected contents when reading a block.