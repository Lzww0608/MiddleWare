#include <stdio.h>
#include <stdlib.h>
#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <time.h>

#include "reactor.h"
#include "adapter_async.h"

static reactor_t *R;
static int cnt, before, num;

int current_tick() {
    int t = 0;
    struct timespec ti;
	clock_gettime(CLOCK_MONOTONIC, &ti);
	t = (int)ti.tv_sec * 1000;
	t += ti.tv_nsec / 1000000;
    return t;
}

void getCallback(redisAsyncContext *c, void *r, void *privdata) {
    redisReply *reply = r;
    if (reply == NULL) return;
    printf("argv[%s]: %lld\n", (char*)privdata, reply->integer);

    /* Disconnect after receiving the reply to GET */
    cnt++;
    if (cnt == num) {
        int used = current_tick()-before;
        printf("after %d exec redis command, used %d ms\n", num, used);
        redisAsyncDisconnect(c);
    }
}


void connectCallback(const redisAsyncContext *c, int status) {
    if (status != REDIS_OK) {
        printf("Error: %s\n", c->errstr);
        stop_eventloop(R);
        return;
    }
    printf("Connected...\n");
}

void disconnectCallback(const redisAsyncContext *c, int status) {
    if (status != REDIS_OK) {
        printf("Error: %s\n", c->errstr);
        stop_eventloop(R);
        return;
    }

    printf("Disconnected...\n");
    stop_eventloop(R);
}

int main(int argc, char **argv) {
     // 创建一个新的异步 Redis 上下文
    redisAsyncContext *c = redisAsyncConnect("127.0.0.1", 6379);
    if (c->err) {
        // 处理错误
        printf("Error: %s\n", c->errstr);
        return 1;
    }
	// 设置 AUTH 命令，使用密码 "123456"
    const char *password = "123456";
    redisAsyncCommand(c, getCallback, NULL, "AUTH %s", password);
	
    R = create_reactor();
    redisAttach(R, c);
    
    redisAsyncSetConnectCallback(c, connectCallback);
    redisAsyncSetDisconnectCallback(c, disconnectCallback);

    before = current_tick();
    num = (argc > 1) ? atoi(argv[1]) : 1000;

    for (int i = 0; i < num; i++) {
        redisAsyncCommand(c, getCallback, "count", "INCR counter");
    }
    
    eventloop(R);

    release_reactor(R);
    return 0;
}
