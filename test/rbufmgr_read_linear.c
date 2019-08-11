// SPDX-License-Identifier: MIT
/* Non-linear ring buffer block management test
 *
 * SPDX-FileCopyrightText: 2017, Sven Eckelmann <sven@narfation.org>
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

	assert(!simple_buffer_empty(sbuf));
	assert(!ring_buffer_empty(rbuf));

	simple_buffer_read(sbuf, &sentry);
	ring_buffer_read(rbuf, &rentry);

	assert(sentry.valid == rentry.valid);
	assert(sentry.seqno == rentry.seqno);

	assert(simple_buffer_empty(sbuf));
	assert(ring_buffer_empty(rbuf));
}

void test(size_t bufsize)
{
	struct simple_buffer sbuf;
	struct ring_buffer rbuf;
	size_t i;

	if (bufsize == 0)
		return;

	simple_buffer_init(&sbuf, bufsize);
	ring_buffer_init(&rbuf, bufsize);

	for (i = 1; i < 1000; i++) {
		simple_buffer_add(&sbuf, i);
		ring_buffer_add(&rbuf, i);

		read_compare(&sbuf, &rbuf);
		check_buffers(&sbuf, &rbuf);
	}

	ring_buffer_free(&rbuf);
	simple_buffer_free(&sbuf);
}

int main(void)
{
	size_t i;

	for (i = 0; i < 50; i++)
		test(getnum());

	return 0;
}
