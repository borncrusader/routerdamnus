#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>

int main(int argc, char *argv[])
{
  int sock, port, bytes_rcvd;
  struct sockaddr_in src_addr, dest_addr;
  int addr_size;
  unsigned char buf[1400];

  port = atoi(argv[1]);

  if((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
    perror("server: socket");
    return 1;
  }

  src_addr.sin_family = AF_INET;
  src_addr.sin_port = htons(port);
  src_addr.sin_addr.s_addr = INADDR_ANY;

  if(bind(sock, (struct sockaddr*)&src_addr, sizeof(src_addr)) == -1) {
    close(sock);
    perror("server: bind");
    return 1;
  }

  addr_size = sizeof(dest_addr);

  bytes_rcvd = recvfrom(sock,buf,1400,0,(struct sockaddr*)&dest_addr,&addr_size);

  printf("%d %d %f\n",
         *((int*)buf),
         *((short*)(buf+sizeof(int))),
         *((float*)(buf+sizeof(int)+sizeof(short))));

  close(sock);

  return 0;
}
