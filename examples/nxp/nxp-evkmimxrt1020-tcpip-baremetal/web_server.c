// Copyright (c) 2021 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"
#include "hal.h"
#include "mg_web_server.h"

static const char *s_listening_address = "http://0.0.0.0:80";
int mg_led_state = MG_LED_STANDBY;

static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
	 unsigned int rt = mg_millis()/1000;
	 unsigned int rt_h = rt / 3600;
	 unsigned int rt_m = (rt - 3600 * rt_h) / 60;
	 unsigned int rt_s = rt - 3600 * rt_h - 60 * rt_m;
	 mg_http_reply(c, 200, "", "hello Mongoose!\nRuntime: %5u:%2u'%2u hms", rt_h, rt_m, rt_s);
  }
  (void) fn_data;
  (void) ev_data;
}

void mg_run_server(struct mg_mgr *mgr, struct mg_tcpip_if* mif, int *mg_led_state) {
  MG_INFO(("Starting Mongoose v%s", MG_VERSION));
  MG_INFO(("Listening on     : %s", s_listening_address));

  struct mg_connection *c;

  if ((c = mg_http_listen(mgr, s_listening_address, cb, mgr)) == NULL) {
	MG_ERROR(("Cannot listen on %s. Use http://ADDR:PORT or :PORT",
			  s_listening_address));
	exit(EXIT_FAILURE);
  }

  while (c) {
	  mg_mgr_poll(mgr, 10); // Infinite event loop
	  if (mif->state == MIP_STATE_DOWN) {
		  *mg_led_state = MG_LED_ERROR;
	  }
	  if (mif->state == MIP_STATE_UP) {
		  *mg_led_state = MG_LED_STANDBY;
	  }
	  if (mif->state == MIP_STATE_READY) {
		  if (!mgr->conns) {
			  *mg_led_state = MG_LED_STANDBY;
		  } else if (mgr->conns) {
			  *mg_led_state = MG_LED_LISTEN;
			  if (mgr->conns->next)
				  *mg_led_state = MG_LED_CONNECT;
		  }
	  }
  }
}

/*
 * Led blink status:
 *  - Slow and regular: Standby (no connection)
 *  - Short tick every second: server listening
 *  - Two short ticks every second: server connection(s)
 *  - Fast and regular: no link
 */

void led_blink_cb(void *arg) {
	const int init = -1;
	static int i = init;
	static int prev_state = 0;
	i++;
	if (prev_state != mg_led_state) {
		prev_state = mg_led_state;
		i = 0;
	}
	switch (mg_led_state) {
		case MG_LED_STANDBY:
			if (i == 10) {
				i = init;
				toggle_user_led();
			}
			break;
		case MG_LED_CONNECT:
			if (!i) {
				set_user_led(1);
			} else if (i == 2) {
				set_user_led(0);
			} else if (i == 4) {
				set_user_led(1);
			} else if (i == 7) {
				set_user_led(0);
			}
			 else if (i == 19) {
				 i = init;
			}
			break;
		case MG_LED_ERROR:
			if (!i) {
				set_user_led(1);
			} else if (i == 3) {
				set_user_led(0);
			}
			 else if (i == 5) {
				 i = init;
			}
			break;
		case MG_LED_LISTEN:
			if (!i) {
				set_user_led(1);
			} else if (i == 5) {
				set_user_led(0);
			}
			 else if (i == 23) {
				 i = init;
			}
			break;
		default:
			break;
	}
  (void) arg;
}
