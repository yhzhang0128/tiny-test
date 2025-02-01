/*
 * (C) 2017, Cornell University
 * All rights reserved.
 */

/* Author: Robbert van Renesse, August 2015
 *
 * This code implements a block store stored in memory
 * file system:
 *
 *		inode_intf ramdisk_init(block_t *blocks, block_no nblocks)
 *			Create a new block store, stored in the array of blocks
 *			pointed to by 'blocks', which has nblocks blocks in it.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inode.h>

struct ramdisk_state {
	block_t *blocks;
	block_no nblocks;
};


static int ramdisk_getsize(inode_intf this_bs, unsigned int ino){
	struct ramdisk_state *rs = this_bs->state;

	if (ino != 0) {
		fprintf(stderr, "!!ramdisk_getsize: ino != 0 not supported\n");
		return -1;
	}

	return rs->nblocks;
}

static int ramdisk_setsize(inode_intf this_bs, unsigned int ino, block_no nblocks){
	struct ramdisk_state *rs = this_bs->state;

	int before = rs->nblocks;
	rs->nblocks = nblocks;
	return before;
}

static int ramdisk_read(inode_intf this_bs, unsigned int ino, block_no offset, block_t *block){
	struct ramdisk_state *rs = this_bs->state;

	if (ino != 0) {
		fprintf(stderr, "!!ramdisk_getsize: ino != 0 not supported\n");
		return -1;
	}

	if (offset >= rs->nblocks) {
		fprintf(stderr, "ramdisk_read: bad offset %u\n", offset);
		return -1;
	}
	memcpy(block, &rs->blocks[offset], BLOCK_SIZE);
	return 0;
}

static int ramdisk_write(inode_intf this_bs, unsigned int ino, block_no offset, block_t *block){
	struct ramdisk_state *rs = this_bs->state;

	if (ino != 0) {
		fprintf(stderr, "!!ramdisk_getsize: ino != 0 not supported\n");
		return -1;
	}

	if (offset >= rs->nblocks) {
		fprintf(stderr, "ramdisk_write: bad offset\n");
		return -1;
	}
	memcpy(&rs->blocks[offset], block, BLOCK_SIZE);
	return 0;
}

inode_intf ramdisk_init(block_t *blocks, block_no nblocks){
	struct ramdisk_state *rs = malloc(sizeof(struct ramdisk_state));

	rs->blocks = blocks;
	rs->nblocks = nblocks;

	inode_intf this_bs = malloc(sizeof(struct inode_store));
	this_bs->state = rs;
	this_bs->getsize = ramdisk_getsize;
	this_bs->setsize = ramdisk_setsize;
	this_bs->read = ramdisk_read;
	this_bs->write = ramdisk_write;
	return this_bs;
}
