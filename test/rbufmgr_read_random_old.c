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
			seqno -= bufsize - 1;

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
