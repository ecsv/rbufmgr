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

#ifndef __RBUFMGR_SIMPLE_BUFFER_H__
#define __RBUFMGR_SIMPLE_BUFFER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct bufentry;

struct simple_buffer {
	struct bufentry *buffer;
	size_t max_blocks;
	size_t num_blocks;
	uint32_t write_seqno;
};

void simple_buffer_init(struct simple_buffer *sbuf, size_t size);
void simple_buffer_free(struct simple_buffer *sbuf);

void simple_buffer_add(struct simple_buffer *sbuf, uint32_t seqno);
bool simple_buffer_empty(const struct simple_buffer *sbuf);
void simple_buffer_read(struct simple_buffer *sbuf, struct bufentry *entry);

#ifdef __cplusplus
}
#endif

#endif /* __RBUFMGR_SIMPLE_BUFFER_H__ */
