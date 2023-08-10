#include "tuyalink_core.h"

#ifndef TUYA_CONNECT_H
#define TUYA_CONNECT_H

#define DATA_LEN 30

struct Device
{
  char productId[DATA_LEN];
  char deviceId[DATA_LEN];
  char deviceSecret[DATA_LEN];
};

int tuya_connect(tuya_mqtt_context_t *client, char dId[], char dSecret[]);
void send_memory_usage_to_tuya(tuya_mqtt_context_t *client, double memory_usage, char deviceId[]);

#endif
