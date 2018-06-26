/*
  This exmple program provides a trivial server program that listens for TCP
  connections on port 9995.  When they arrive, it writes a short message to
  each client connection, and closes each connection once it is flushed.

  Where possible, it exits cleanly in response to a SIGINT (ctrl-c).
*/


#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#ifndef WIN32
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#include <sys/socket.h>
#endif

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <netinet/in.h>

static const char MESSAGE[] = "Hello, World!\n";

static const int PORT = 9995;



#define  debug(x)   #x

static void bufferevent_readcb(struct bufferevent *bev,void *ctx){

            printf("hello world");

			if(bufferevent_get_input(bev)==NULL){
				 printf("bufferevent_get_input==NULL");
				 
			}else{
			     //printf("receive%s",bufferevent_get_input(bev));
			     // struct evbuffer *buf=bufferevent_get_input(bev);
				 //char *preq=evbuffer_readline(buf);
				 char buffer[1024];
				 memset(buffer,0,sizeof(buffer));
				 bufferevent_read(bev,buffer,1024);
				 
				 printf("receive echo data: %s",buffer);
			}
}

//static void bufferevent_writecb(struct bufferevent *bev,void *ctx)
//{
            

//}
static void read_cd(int fd,void *ctx){

           printf("into read_cb");
           char input[1024];
		   memset(input,0,sizeof(input));
		   int res=read(fd,input,sizeof(input));
		   if(res<0){
		   	  printf("read something error!");
		   	  return;
		   }

		   struct bufferevent *bev=(struct bufferevent *)(ctx);
		   if(!bev){
		   	  printf("read something error!");
		   	  return;
		   }
           
		   bufferevent_write(bev,input,strlen(input));
}


int
main(int argc, char **argv)
{
	struct event_base *base;
	struct bufferevent *p_event;
	evutil_socket_t socketfd;

	struct sockaddr_in sin;
#ifdef WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif
    //socketfd = socket(AF_INET,SOCK_STREAM,0);
	base = event_base_new();
	if (!base) {
		fprintf(stderr, "Could not initialize libevent!\n");
		return 1;
	}
    
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);
    sin.sin_addr.s_addr=inet_addr("172.16.103.125");

	p_event=bufferevent_socket_new(base,-1,BEV_OPT_CLOSE_ON_FREE);
	if(p_event==NULL)
	{
	  printf("socket_new error!");
	  //exit(-1);
	  return -1;
	}
	
	socketfd=bufferevent_socket_connect(p_event,(struct sockaddr*)&sin,
	    sizeof(sin));

	bufferevent_setcb(p_event, bufferevent_readcb, NULL, NULL,base);
    bufferevent_enable(p_event,EV_READ|EV_PERSIST);//active callback 
    bufferevent_enable(p_event,EV_WRITE);

	printf("bufferevent......");
	
	struct bufferevent *stdin_event=bufferevent_new(STDIN_FILENO,read_cd,read_cd,NULL,p_event);
	bufferevent_base_set(base,stdin_event);
	bufferevent_enable(stdin_event,EV_READ|EV_PERSIST);
	bufferevent_disable(stdin_event,EV_WRITE);
	
	if(socketfd<0)
	{
	    printf("socket_connect!");
	    return -2;
	}


	
	event_base_dispatch(base); //loop

	
	bufferevent_free(p_event);
	//event_free(pevent);
	bufferevent_free(stdin_event);
	event_base_free(base);
	

	printf("done\n");
	return 0;
}


