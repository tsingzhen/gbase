#ifndef CONNECTOR_H_
#define CONNECTOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "os/os_def.h"
#include "net/sock.h"
#include "net/reactor.h"
#include "ds/connbuffer.h"

/* return buffer size processed, return -1 means process fail, reactor will close it */
typedef int (*connector_read_func)(int fd, const char* buffer, int buflen);
typedef void (*connector_close_func)(int fd);

struct connector_t;

struct connector_t* connector_init(struct reactor_t* r, 
        connector_read_func read_cb, 
        connector_close_func close_cb,
        struct connbuffer_t* read_buf,
        struct connbuffer_t* write_buf,
        int fd);
int connector_release(struct connector_t* con);
int connector_fd(struct connector_t* con);
int connector_start(struct connector_t* con);
int connector_stop(struct connector_t* con);

/*
*    return < 0 success
*    return >=0, send bytes, maybe < buflen, some bytes full discard
*/
int connector_send(struct connector_t* con, const char* buffer, int buflen);


#ifdef __cplusplus
}
#endif

#endif // CONNECTOR_H_


