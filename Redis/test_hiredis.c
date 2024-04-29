#include <hiredis/hiredis.h>
#include <stdio.h>

int main() {
    redisContext *c = redisConnect("127.0.0.1", 6379);
    if (c != NULL && c->err) {
        printf("Error: %s\n", c->errstr);
    } else {
        printf("Connected to Redis\n");
        redisReply *reply = redisCommand(c, "PING");
        printf("PING: %s\n", reply->str);
        freeReplyObject(reply);
        redisFree(c);
    }
    return 0;
}
