/*
 * (C) 2025, Cornell University
 * All rights reserved.
 *
 * Description: a dummy file system illustrating the concept of "inode"
 */

#ifdef MKFS
#include <stdio.h>
#include <sys/types.h>
#else
#include "egos.h"
#endif
#include "inode.h"
#include <stdlib.h>

#define DUMMY_DISK_OFFSET(ino, offset) ino * 2 + offset

int mydisk_read(inode_intf self, uint ino, uint offset, block_t* block) {
    if (offset >= 2) return -1;
    inode_intf below = self->state;
    return below->read(below, 0, DUMMY_DISK_OFFSET(ino, offset), block);
}

int mydisk_write(inode_intf self, uint ino, uint offset, block_t* block) {
    if (offset >= 2) return -1;
    inode_intf below = self->state;
    return below->write(below, 0, DUMMY_DISK_OFFSET(ino, offset), block);
}

int mydisk_getsize(inode_intf self, uint ino) {
#ifdef MKFS
    fprintf(stderr, "mydisk_getsize not implemented");
    while (1);
#else
    FATAL("mydisk_getsize not implemented");
#endif
}

int mydisk_setsize(inode_intf self, uint ino, uint nblocks) {
#ifdef MKFS
    fprintf(stderr, "mydisk_setsize not implemented");
    while (1);
#else
    FATAL("mydisk_setsize not implemented");
#endif
}

inode_intf mydisk_init(inode_intf below, uint below_ino) {
    inode_intf self = malloc(sizeof(struct inode_store));
    self->getsize   = mydisk_getsize;
    self->setsize   = mydisk_setsize;
    self->read      = mydisk_read;
    self->write     = mydisk_write;
    self->state     = below;
    return self;
}

int mydisk_create(inode_intf below, uint below_ino, uint ninodes) {
    return 0;
}
