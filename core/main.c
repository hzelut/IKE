#include <unistd.h>

#include "daemon.h"

int main() {
	daemon_create();

	sam_init_sas(DAEMON.SAM, "conf/local.conf");


	running();
	daemon_free(&DAEMON);
	return 0;
}
