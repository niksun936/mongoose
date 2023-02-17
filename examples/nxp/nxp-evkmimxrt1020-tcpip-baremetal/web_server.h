// Copyright (c) 2021 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

#ifndef WEB_SERVER_H_
#define WEB_SERVER_H_

void mg_run_server(struct mg_mgr *, struct mg_tcpip_if *);
enum mg_led_state_s {MG_LED_STANDBY, MG_LED_CONNECT, MG_LED_ERROR, MG_LED_LISTEN};
void led_blink_cb(void *);

#endif /* WEB_SERVER_H_ */
