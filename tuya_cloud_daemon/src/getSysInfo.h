#ifndef GET_SYS_INFO_H
#define GET_SYS_INFO_H

int get_memory_usage(struct ubus_context *ctx, uint32_t id, double *memory_usage);

#endif