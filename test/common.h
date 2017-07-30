/* Non-linear ring buffer block management test
 *
 * Copyright (c) 2012-2017, Sven Eckelmann <sven@narfation.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
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
		assert(rbuf->rbufmgr.read_index == rbufmgr_peak(&rbuf->rbufmgr));

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
