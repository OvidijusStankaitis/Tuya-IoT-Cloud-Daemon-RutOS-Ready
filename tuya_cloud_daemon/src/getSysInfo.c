#include <libubox/blobmsg_json.h>
#include <libubus.h>
#include <syslog.h>

#include "getSysInfo.h"

struct MemData {
	int total;
	int free;
	int shared;
	int buffered;
};

enum {
	TOTAL_MEMORY,
	FREE_MEMORY,
	SHARED_MEMORY,
	BUFFERED_MEMORY,
	__MEMORY_MAX,
};

enum {
	MEMORY_DATA,
	__INFO_MAX,
};

static const struct blobmsg_policy memory_policy[__MEMORY_MAX] = {
	[TOTAL_MEMORY]	  = { .name = "total", .type = BLOBMSG_TYPE_INT64 },
	[FREE_MEMORY]	  = { .name = "free", .type = BLOBMSG_TYPE_INT64 },
	[SHARED_MEMORY]	  = { .name = "shared", .type = BLOBMSG_TYPE_INT64 },
	[BUFFERED_MEMORY] = { .name = "buffered", .type = BLOBMSG_TYPE_INT64 },
};

static const struct blobmsg_policy info_policy[__INFO_MAX] = {
	[MEMORY_DATA] = { .name = "memory", .type = BLOBMSG_TYPE_TABLE },
};

static void board_cb(struct ubus_request *req, int type, struct blob_attr *msg)
{
	if (!msg) {
		syslog(LOG_ERR, "Received NULL message in board_cb");
		return;
	}

	struct MemData *memoryData = (struct MemData *)req->priv;
	struct blob_attr *tb[__INFO_MAX];
	struct blob_attr *memory[__MEMORY_MAX];

	blobmsg_parse(info_policy, __INFO_MAX, tb, blob_data(msg), blob_len(msg));

	if (!tb[MEMORY_DATA]) {
		syslog(LOG_ERR, "No memory data received");
		return;
	}

	blobmsg_parse(memory_policy, __MEMORY_MAX, memory, blobmsg_data(tb[MEMORY_DATA]),
		      blobmsg_data_len(tb[MEMORY_DATA]));

	if (!memory[TOTAL_MEMORY] || !memory[FREE_MEMORY] || !memory[SHARED_MEMORY] || !memory[BUFFERED_MEMORY]) {
		syslog(LOG_ERR, "Incomplete memory data received");
		return;
	}

	memoryData->total    = blobmsg_get_u64(memory[TOTAL_MEMORY]);
	memoryData->free     = blobmsg_get_u64(memory[FREE_MEMORY]);
	memoryData->shared   = blobmsg_get_u64(memory[SHARED_MEMORY]);
	memoryData->buffered = blobmsg_get_u64(memory[BUFFERED_MEMORY]);
}

int get_memory_usage(struct ubus_context *ctx, uint32_t id, double *memory_usage)
{
	if (!ctx) {
		syslog(LOG_ERR, "NULL ubus context provided to get_memory_usage");
		return 1;
	}

	struct MemData memory = { 0 };

	if (ubus_invoke(ctx, id, "info", NULL, board_cb, &memory, 1000)) {
		syslog(LOG_ERR, "cannot request memory info from ubus");
		return 1;
	}

	if (memory.total <= 0 || memory.free <= 0) {
		syslog(LOG_ERR, "Invalid memory data received: total=%d, free=%d", memory.total, memory.free);
		return 1;
	}

	*memory_usage = (memory.total - memory.free) / (1024.0 * 1024.0 * 1024.0);
	syslog(LOG_INFO, "Calculated used memory: %0.2f GB", memory_usage);
	return 0;
}