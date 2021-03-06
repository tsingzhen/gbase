#include "net/reactor.h"
#include "net/reactor_inner.h"

struct reactor_t* reactor_init()
{
    int ret;
    struct reactor_t* reactor = (struct reactor_t*)MALLOC(sizeof(struct reactor_t));
    if (!reactor) return NULL;

    reactor->name = NULL;
    reactor->impl = 0;
    reactor->data = 0;

#if defined(OS_LINUX)
    reactor->impl = &reactor_epoll;
#elif defined(OS_MAC)
    reactor->impl = &reactor_kqueue;
#else
    reactor->impl = &reactor_select;
#endif
    ret = reactor->impl->init(reactor);
    if (ret < 0) {
        FREE(reactor);
        return NULL;
    }
    return reactor;
}

void reactor_release(struct reactor_t* reactor)
{
    if (reactor) {
        reactor->impl->release(reactor);
        FREE(reactor);
    }
}

int reactor_register(struct reactor_t* reactor, struct handler_t* h, int events)
{
    return reactor->impl->add(reactor, h, events);
}

int reactor_unregister(struct reactor_t* reactor, struct handler_t* h)
{
    return reactor->impl->remove(reactor, h);
}

int reactor_modify(struct reactor_t* reactor, struct handler_t* h, int events)
{
    return reactor->impl->modify(reactor, h, events);
}

int reactor_dispatch(struct reactor_t* reactor, int ms)
{
    return reactor->impl->dispatch(reactor, ms);
}


