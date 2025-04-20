#include <stdlib.h>
#include <ctype.h>

#include "log.h"
#include "daemon.h"

log_t* log_check(LOG_LEVEL level) {
  if(DAEMON.LOG == NULL || level < DAEMON.LOG->level)
    return NULL;

  return DAEMON.LOG;
}

log_t* log_create() {
  log_t* self = calloc(1, sizeof(log_t));
	pthread_mutex_init(&self->mutex, NULL);
  self->level = LL_ALL;
  self->type = LT_STD;
  self->out = stdout;

  return self;
}

void logging(LOG_LEVEL level, const char* module, const char* fmt, ...) {
  log_t* self = log_check(level);
  if(self == NULL)
    return;

  va_list args;
  va_start(args, fmt);

	pthread_mutex_lock(&self->mutex);
  fprintf(self->out, "[%s] ", module);
  vfprintf(self->out, fmt, args);
  fprintf(self->out, "\n");
	pthread_mutex_unlock(&self->mutex);

  va_end(args);
}

void logging_buf(LOG_LEVEL level, const char* module, buffer_t* buf) {
  logging_hex(level, module, buf->data, buf->capacity);
}

void logging_hex(LOG_LEVEL level, const char* module, void* data, int size) {
  log_t* self = log_check(level);
  if(self == NULL)
    return;

  // width is fixed size; 16
  char hbuf[49] = {'\0',};  // hex buffer
  char cbuf[33] = {'\0',};  // char buffer

  // i = chunks's index
  // j = hex&char buf's index
  for(int i = 0, j = 0; i < size; i++, j++) {
    unsigned char c = 0xFF & ((char*)data)[i];

    // hex
    sprintf(&hbuf[j*3], "%02X ", c);

    // character
    if(isprint(c))
      sprintf(&cbuf[j*2], " %c", c);
    else
      sprintf(&cbuf[j*2], " .");

    // end of width OR chunk
    if(j == 15 || i+1 == size) {
      j++;
      hbuf[j*3] = cbuf[j*2] = '\0';
      j = -1;
      fprintf(self->out, "[%s] %-48s %s\n", module, hbuf, cbuf);
    }
  }
}
