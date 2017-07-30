/* Non-linear ring buffer block management test
 *
 * Copyright (c) 2017, Sven Eckelmann <sven@narfation.org>
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

#include "ring_buffer.h"
#include <assert.h>
#include <stdlib.h>

#include "common.h"

void ring_buffer_init(struct ring_buffer *rbuf, size_t size)
{
	size_t i;

	rbuf->buffer = (struct bufentry *)malloc(size * sizeof(*rbuf->buffer));
	assert(rbuf->buffer);

	rbufmgr_init(&rbuf->rbufmgr, size, 1);

	for (i = 0; i < size; i++) {
		rbuf->buffer[i].valid = false;
		rbuf->buffer[i].seqno = 0;
	}
}

void ring_buffer_free(struct ring_buffer *rbuf)
{
	free(rbuf->buffer);
}

void ring_buffer_add(struct ring_buffer *rbuf, uint32_t seqno)
{
	size_t read_index;
	size_t lost_read_blocks;
	size_t i;
	size_t ri;
	struct bufentry *rentry;

	if (rbufmgr_outdated(&rbuf->rbufmgr, seqno))
		return;

	/* invalidate removed entries */
	rbufmgr_shift_distance(&rbuf->rbufmgr, seqno, &read_index,
			       &lost_read_blocks);

	for (i = 0; i < lost_read_blocks; i++) {
		ri = i + rbuf->rbufmgr.read_index;
		ri %= rbuf->rbufmgr.max_blocks;

		rentry = &rbuf->buffer[ri];
		rentry->valid = false;
		rentry->seqno = 0;
	}

	/* add actual seqno */
	ri = rbufmgr_insert(&rbuf->rbufmgr, seqno);
	rentry = &rbuf->buffer[ri];
	rentry->valid = true;
	rentry->seqno = seqno;
}

bool ring_buffer_empty(const struct ring_buffer *rbuf)
{
	return rbufmgr_empty(&rbuf->rbufmgr);
}

void ring_buffer_read(struct ring_buffer *rbuf, struct bufentry *entry)
{
	size_t i;

	if (ring_buffer_empty(rbuf))
		return;

	i = rbufmgr_read(&rbuf->rbufmgr);
	*entry = rbuf->buffer[i];

	rbuf->buffer[i].valid = false;
}
