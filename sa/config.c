#include <stdio.h>
#include <string.h>
#include <libconfig.h>

#include "config.h"
#include "log.h"

static const char* MM="CNF";

configure_t* cnf_parse(const char* filename, int* count) {
	configure_t* self = NULL;
	config_t cf;
	config_setting_t *left, *right;
	const char* buf;

	config_init(&cf);

	if(!config_read_file(&cf, filename)) {
		logging(LL_ERR, MM, "Failed read config file");
		config_destroy(&cf);
		return NULL;
	}

	config_setting_t* peers = config_lookup(&cf, "peers");
	if(peers != NULL) {
		*count = config_setting_length(peers);

		self = calloc(*count, sizeof(configure_t));
		for(int i = 0; i < *count; i++) {
			logging(LL_DBG, MM, "[peer %d] Parsing started", i+1);
			config_setting_t* cf_p = config_setting_get_elem(peers, i);

			left = config_setting_lookup(cf_p, "local");
			if(left != NULL) {
				logging(LL_DBG, MM, "  [local]");
				if(config_setting_lookup_string(left, "ip", &buf)) {
					self[i].local.addr = net_stoa(buf);
					logging(LL_DBG, MM, "    ip: %s", buf);
				}
			}

			right = config_setting_lookup(cf_p, "remote");
			if(right != NULL) {
				logging(LL_DBG, MM, "  [remote]");
				if(config_setting_lookup_string(right, "ip", &buf)) {
					self[i].remote.addr = net_stoa(buf);
					logging(LL_DBG, MM, "    ip: %s", buf);
				}
			}

			if(config_setting_lookup_string(cf_p, "secret", &buf)) {
				self[i].secret = calloc(strlen(buf), sizeof(char));
				memcpy(self[i].secret, buf, strlen(buf));
				logging(LL_DBG, MM, "  secret: %s", self[i].secret);
			}

			logging(LL_DBG, MM, "Parsing finished", i+1);
		}
	}

	return self;
};
