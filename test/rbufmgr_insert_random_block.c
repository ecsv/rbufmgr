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

void test(size_t bufsize)
{
	struct simple_buffer sbuf;
	struct ring_buffer rbuf;
	size_t i;
	uint32_t seqno = 1;
	size_t blocksize;
	size_t j;

	if (bufsize == 0)
		return;

	simple_buffer_init(&sbuf, bufsize);
	ring_buffer_init(&rbuf, bufsize);

	for (i = 1; i < 100; i++) {
		seqno += getnum();

		blocksize = getnum();
		for (j = 0; j < blocksize; j++) {
			simple_buffer_add(&sbuf, seqno);
			ring_buffer_add(&rbuf, seqno);
			check_buffers(&sbuf, &rbuf);
			seqno++;
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
