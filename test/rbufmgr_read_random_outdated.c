// SPDX-License-Identifier: MIT
/* Non-linear ring buffer block management test
 *
 * SPDX-FileCopyrightText: Sven Eckelmann <sven@narfation.org>
 */

#include "../rbufmgr.h"
#include <assert.h>

#include "common.h"
#include "ring_buffer.h"
#include "simple_buffer.h"

void read_compare(struct simple_buffer *sbuf, struct ring_buffer *rbuf)
{
	struct bufentry sentry;
	struct bufentry rentry;

	if (!simple_buffer_empty(sbuf)) {
		assert(!simple_buffer_empty(sbuf));
		assert(!ring_buffer_empty(rbuf));

		simple_buffer_read(sbuf, &sentry);
		ring_buffer_read(rbuf, &rentry);

		assert(sentry.valid == rentry.valid);
		if (sentry.valid)
			assert(sentry.seqno == rentry.seqno);
	}

	assert(simple_buffer_empty(sbuf) == ring_buffer_empty(rbuf));
}

void test(size_t bufsize)
{
	struct simple_buffer sbuf;
	struct ring_buffer rbuf;
	size_t i;
	uint32_t seqno = 1;
	size_t blocksize;
	size_t j;
	uint8_t mode;

	if (bufsize == 0)
		return;

	simple_buffer_init(&sbuf, bufsize);
	ring_buffer_init(&rbuf, bufsize);

	for (i = 1; i < 100; i++) {
		mode = getnum() % 2;
		if (mode == 0)
			seqno += getnum();
		else
			seqno -= getnum();

		blocksize = getnum();
		for (j = 0; j < blocksize; j++) {
			simple_buffer_add(&sbuf, seqno);
			ring_buffer_add(&rbuf, seqno);
			check_buffers(&sbuf, &rbuf);
			seqno++;
		}

		blocksize = getnum();
		for (j = 0; j < blocksize; j++) {
			read_compare(&sbuf, &rbuf);
			check_buffers(&sbuf, &rbuf);
		}
	}

	ring_buffer_free(&rbuf);
	simple_buffer_free(&sbuf);
}

int main(void)
{
	size_t i;

	for (i = 0; i < 10; i++)
		test(getnum());

	return 0;
}
