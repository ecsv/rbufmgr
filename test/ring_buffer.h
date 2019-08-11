/* SPDX-License-Identifier: MIT */
/* Non-linear ring buffer block management test
 *
 * SPDX-FileCopyrightText: 2012-2017, Sven Eckelmann <sven@narfation.org>
 */

#ifndef __RBUFMGR_RING_BUFFER_H__
#define __RBUFMGR_RING_BUFFER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../rbufmgr.h"

struct bufentry;

struct ring_buffer {
	struct bufentry *buffer;
	struct rbufmgr rbufmgr;
};

void ring_buffer_init(struct ring_buffer *rbuf, size_t size);
void ring_buffer_free(struct ring_buffer *rbuf);

void ring_buffer_add(struct ring_buffer *rbuf, uint32_t seqno);
bool ring_buffer_empty(const struct ring_buffer *rbuf);
void ring_buffer_read(struct ring_buffer *rbuf, struct bufentry *entry);


#ifdef __cplusplus
}
#endif

#endif /* __RBUFMGR_RING_BUFFER_H__ */
