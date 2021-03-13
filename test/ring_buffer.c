// SPDX-License-Identifier: MIT
/* Non-linear ring buffer block management test
 *
 * SPDX-FileCopyrightText: Sven Eckelmann <sven@narfation.org>
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
