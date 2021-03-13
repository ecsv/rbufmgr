/* SPDX-License-Identifier: MIT */
/* Non-linear ring buffer block management test
 *
 * SPDX-FileCopyrightText: Sven Eckelmann <sven@narfation.org>
 */

#ifndef __RBUFMGR_COMMON_H__
#define __RBUFMGR_COMMON_H__

#include <stdbool.h>
#include <stdint.h>

#include "ring_buffer.h"
#include "simple_buffer.h"

static __inline__ uint8_t getnum(void)
{
	static uint16_t s1 = UINT16_C(2);
	static uint16_t s2 = UINT16_C(1);
	static uint16_t s3 = UINT16_C(1);

	s1 *= UINT16_C(171);
	s1 %= UINT16_C(30269);

	s2 *= UINT16_C(172);
	s2 %= UINT16_C(30307);

	s3 *= UINT16_C(170);
	s3 %= UINT16_C(30323);

	return s1 ^ s2 ^ s3;
}

static __inline__ uint16_t get_unsigned16(void)
{
	uint16_t x = 0;
	size_t i;

	for (i = 0; i < sizeof(x); i++) {
		x <<= 8;
		x |= getnum();
	}

	return x;
}

struct bufentry {
	bool valid;
	uint32_t seqno;
};

static __inline__ void check_buffers(const struct simple_buffer *sbuf,
				     const struct ring_buffer *rbuf)
{
	size_t i;
	size_t ri;
	struct bufentry *rentry;

	assert(sbuf->max_blocks == rbuf->rbufmgr.max_blocks);
	assert(sbuf->num_blocks == rbuf->rbufmgr.num_blocks);
	assert(sbuf->write_seqno == rbuf->rbufmgr.write_seqno);

	for (i = 0; i < sbuf->num_blocks; i++) {
		assert(rbuf->rbufmgr.read_index == rbufmgr_peek(&rbuf->rbufmgr));

		ri = i + rbuf->rbufmgr.read_index;
		ri %= rbuf->rbufmgr.max_blocks;
		rentry = &rbuf->buffer[ri];

		assert(sbuf->buffer[i].valid == rentry->valid);
		if (rentry->valid) {
			assert(sbuf->buffer[i].seqno == rentry->seqno);
		}
	}
}

#endif /* __RBUFMGR_COMMON_H__ */
