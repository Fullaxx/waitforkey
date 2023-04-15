#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <hiredis/hiredis.h>

unsigned int g_usleep_delay = 10000;

static void check_reply_for_errors(redisContext *c, redisReply *reply)
{
	int doexit = 0;
	if(!reply) { fprintf(stderr, "reply is NULL!\n"); exit(1); }

	switch(c->err) {
		case REDIS_ERR_IO:
			fprintf(stderr, "c->err == REDIS_ERR_IO: %s!\n", strerror(errno));
			doexit = 1;
			break;
		case REDIS_ERR_EOF:
			fprintf(stderr, "c->err == REDIS_ERR_EOF!\n");
			doexit = 1;
			break;
		case REDIS_ERR_PROTOCOL:
			fprintf(stderr, "c->err == REDIS_ERR_PROTOCOL!\n");
			doexit = 1;
			break;
		case REDIS_ERR_OOM:
			fprintf(stderr, "c->err == REDIS_ERR_OOM!\n");
			doexit = 1;
			break;
#ifdef REDIS_ERR_TIMEOUT
		case REDIS_ERR_TIMEOUT:
			fprintf(stderr, "c->err == REDIS_ERR_TIMEOUT!\n");
			doexit = 1;
			break;
#endif
		case REDIS_ERR_OTHER:
			fprintf(stderr, "c->err == REDIS_ERR_OTHER!\n");
			doexit = 1;
			break;
	}
	if(reply->type == REDIS_REPLY_ERROR) {
		fprintf(stderr, "REPLY ERROR: %s!\n", reply->str);
		doexit = 1;
	}
	if(doexit) { exit(1); }
}

static char* waitforkey(redisContext *c, char *key)
{
	redisReply *reply;
	char *kval = NULL;

	do {
#ifdef DEBUG
		printf("GET %s: ", key);
#endif
		reply = redisCommand(c, "GET %s", key);
		check_reply_for_errors(c, reply);
#ifdef DEBUG
		if(reply->str) { printf("%s\n", reply->str); }
		else { printf("(nil)\n"); }
#endif
		if(reply->str) { kval = strdup(reply->str); }
		else { usleep(g_usleep_delay); }
		freeReplyObject(reply);
	} while(!kval);

	return kval;
}

static redisContext* redis_connect(char *connect_str)
{
	unsigned short port = 9;
	redisContext *c = NULL;

	char *colon = strchr(connect_str, ':');
	if(colon) {
		*colon = 0;
		port = atoi(colon+1);
	}

#ifdef DEBUG
	if(port) { printf("Attempting redisConnect(%s,%u) ...\n", connect_str, port); }
	else	{ printf("Attempting redisConnect(%s) ...\n", connect_str); }
#endif

	if(port) { c = redisConnect(connect_str, port); }
	else	{ c = redisConnectUnix(connect_str); }

	if(!c) {
		fprintf(stderr, "Connection error: can't allocate redis context\n");
		return c;
	}

	if(c->err) {
		fprintf(stderr, "Connection error: %s\n", c->errstr);
		redisFree(c);
		c = NULL;
	}

	return c;
}

static void process_environment(void)
{
	char *usleep_delay_str = getenv("USLEEPDELAY");
	if(usleep_delay_str) {
		g_usleep_delay = atoi(usleep_delay_str);
	}
}

static void bail_usage(char *s)
{
	fprintf(stderr, "%s: <REDIS> <KEY>\n", s);
	exit(1);
}

int main(int argc, char *argv[])
{
	if(argc != 3) { bail_usage(argv[0]); }

	process_environment();

	redisContext *c = redis_connect(argv[1]);
	if(!c) { return 1; }

	char *kval = waitforkey(c, argv[2]);
	printf("%s: %s\n", argv[2], kval);
	free(kval);

	redisFree(c);
	return 0;
}
