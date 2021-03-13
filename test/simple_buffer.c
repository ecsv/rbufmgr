// SPDX-License-Identifier: MIT
/* Non-linear ring buffer block management test
 *
 * SPDX-FileCopyrightText: Sven Eckelmann <sven@narfation.org>
 */

#include "simple_buffer.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

void simple_buffer_init(struct simple_buffer *sbuf, size_t size)
{
	size_t i;

	sbuf->buffer = (struct bufentry *)malloc(size * sizeof(*sbuf->buffer));
	assert(sbuf->buffer);

	sbuf->max_blocks = size;
	sbuf->num_blocks = 0;
	sbuf->write_seqno = 0;

	for (i = 0; i < size; i++) {
		sbuf->buffer[i].valid = false;
		sbuf->buffer[i].seqno = 0;
	}
}

void simple_buffer_free(struct simple_buffer *sbuf)
{
	free(sbuf->buffer);
}

static int32_t seqno_distance(uint32_t new_seqno, uint32_t cur_seqno)
{
	return new_seqno - cur_seqno;
}
void simple_buffer_add(struct simple_buffer *sbuf, uint32_t seqno)
{
	uint32_t cur_seqno = sbuf->write_seqno;
	int32_t distance;

	/* ignore sequence numbers older than our start of the buffer */
	distance = seqno_distance(seqno, cur_seqno);
	if (distance <= 0 &&
	    ((uint32_t)-distance) >= sbuf->num_blocks)
		return;

	/* update older entries which are still between read and write index */
	if (distance <= 0) {
		sbuf->buffer[sbuf->num_blocks + distance - 1].valid = true;
		sbuf->buffer[sbuf->num_blocks + distance - 1].seqno = seqno;
		return;
	}

	/* increase buffer until room for new seqno was found */
	while (sbuf->num_blocks < sbuf->max_blocks &&
	       seqno_distance(cur_seqno, seqno) < 0) {
		sbuf->buffer[sbuf->num_blocks].valid = false;
		sbuf->buffer[sbuf->num_blocks].seqno = cur_seqno;
		cur_seqno++;
		sbuf->num_blocks++;
	}

	/* shift buffer until room for new seqno was found */
	while (seqno_distance(cur_seqno, seqno) < 0) {
		memmove(&sbuf->buffer[0], &sbuf->buffer[1],
			(sbuf->num_blocks - 1) * sizeof(*sbuf->buffer));

		sbuf->buffer[sbuf->num_blocks - 1].valid = false;
		sbuf->buffer[sbuf->num_blocks - 1].seqno = cur_seqno;
		cur_seqno++;
	}

	/* insert new entry and update the latest seqno */
	sbuf->buffer[sbuf->num_blocks - 1].valid = true;
	sbuf->buffer[sbuf->num_blocks - 1].seqno = seqno;
	sbuf->write_seqno = seqno;
}

bool simple_buffer_empty(const struct simple_buffer *sbuf)
{
	return sbuf->num_blocks == 0;
}

void simple_buffer_read(struct simple_buffer *sbuf, struct bufentry *entry)
{
	if (simple_buffer_empty(sbuf))
		return;

	*entry = sbuf->buffer[0];

	sbuf->num_blocks--;
	if (sbuf->num_blocks > 0)
		memmove(&sbuf->buffer[0], &sbuf->buffer[1],
			sbuf->num_blocks * sizeof(*sbuf->buffer));
}
