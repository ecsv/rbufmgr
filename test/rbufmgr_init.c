// SPDX-License-Identifier: MIT
/* Non-linear ring buffer block management test
 *
 * SPDX-FileCopyrightText: 2017, Sven Eckelmann <sven@narfation.org>
 */

#include "../rbufmgr.h"
#include <assert.h>

#include "common.h"

int main(void)
{
	struct rbufmgr rbufmgr;

	rbufmgr_init(&rbufmgr, 16, 1);

	assert(rbufmgr.max_blocks == 16);
	assert(rbufmgr.num_blocks == 0);
	assert(rbufmgr.write_seqno == 0);
	assert(rbufmgr.write_index == 0);
	assert(rbufmgr.read_index == 0);

	return 0;
}
