/* SPDX-License-Identifier: MIT */
/* Non-linear ring buffer block management test
 *
 * SPDX-FileCopyrightText: Sven Eckelmann <sven@narfation.org>
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
