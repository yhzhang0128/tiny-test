#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <inode.h>


inode_intf ramdisk_init(block_t *blocks, block_no nblocks);
inode_intf mydisk_init(inode_intf below, uint below_ino);
inode_intf checkdisk_init(inode_intf below, const char *descr);
int mydisk_create(inode_intf below, uint below_ino, uint ninodes);

#define DISK_SIZE  16
#define MAX_INODES 6

static block_t blocks[DISK_SIZE]; // blocks for ram_disk

static void panic(const char *s) {
    fprintf(stderr, "!!PANIC: %s\n", s);
    exit(1);
}

void dump_blocks(){
    for (int i = 0; i < DISK_SIZE; i++) {
        printf("%2d:", i);
        block_no *b = (block_no *) &blocks[i];
        for (int j = 0; j < 8; j++) {
            printf(" %2d", *b++);
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {
    inode_intf disk = ramdisk_init(blocks, DISK_SIZE);
    if (mydisk_create(disk, 0, MAX_INODES) < 0) {
        panic("trace: can't create mydisk file system");
    }
    inode_intf fdisk = mydisk_init(disk, 0);
    inode_intf cdisk = checkdisk_init(fdisk, "fat");

    for (int cnt = 1;; cnt++) {
        dump_blocks();

        char line[128];
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        char cmd;
        unsigned int inode, bno, data;
        unsigned int nargs = sscanf(line, "%c:%u:%u:%u", &cmd, &inode, &bno, &data);
        if (nargs == 0 || cmd == '\n') {
            continue;
        }
        if (cmd == 'Q') {
            break;
        }
        static block_t block;
        int result;
        switch (cmd) {
        case 'R':
            assert(nargs == 3 || nargs == 4);
            result = (*cdisk->read)(cdisk, inode, bno, &block);
            if (result < 0) {
                fprintf(stderr, "!!ERROR: tracedisk_run: read(%u, %u) failed\n", inode, bno);
                break;
            }
            if (nargs == 3 &&
                (
                    (((unsigned int *) &block)[0] != inode && ((unsigned int *) &block)[0] != 0)
                    || (((unsigned int *) &block)[1] != bno && ((unsigned int *) &block)[1] != 0)
                ))
            {
                fprintf(stderr, "!!ERROR: tracedisk_run: unexpected content %u %u %u %u\n", inode, bno, ((unsigned int *) &block)[0], ((unsigned int *) &block)[1]);
            }
            if ((nargs == 4) && (((unsigned int *) &block)[2] != data)) {
                fprintf(stderr, "!!ERROR @line%u: tracedisk_run: read %u %u: %u != %u\n",
                                        cnt, inode, bno, data, ((unsigned int *) &block)[2]);
            }
            break;
        case 'W':
            assert(nargs == 3 || nargs == 4);
            ((unsigned int *) &block)[0] = inode;
            ((unsigned int *) &block)[1] = bno;
            if (nargs == 4) {
                ((unsigned int *) &block)[2] = data;
            } else {
                ((unsigned int *) &block)[2] = cnt; // line# by default
            }
            result = (*cdisk->write)(cdisk, inode, bno, &block);
            if (result < 0) {
                fprintf(stderr, "!!ERROR: tracedisk_run: write(%u, %u) failed\n", inode, bno);
                break;
            }
            break;
        case 'S':
            assert(nargs == 3);
            result = (*cdisk->setsize)(cdisk, inode, bno);
            if (result < 0) {
                fprintf(stderr, "!!ERROR: tracedisk_run: setsize(%u, %u) failed\n", inode, bno);
                break;
            }
            break;
        case 'G':
            assert(nargs == 2 || nargs == 3);
            result = (*cdisk->getsize)(cdisk, inode);
            if (result < 0) {
                fprintf(stderr, "!!ERROR: tracedisk_run: getsize(%u, %u) failed\n", inode, bno);
                break;
            }
            if (nargs == 2) {
                printf("getsize --> %u\n", result);
            }
            else if ((nargs == 3) && (result != bno)) {
                fprintf(stderr, "!!ERROR @line%u: tracedisk_run: getsize %u: %u != %u\n",
                                        cnt, inode, bno, result);
            }
            break;
        default:
            printf("unknown command\n");
        }
    }

    return 0;
}
