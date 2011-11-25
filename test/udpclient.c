#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>

int construct_buffer(void *buf)
{                                                          
  unsigned int s = 12345;                                  
  short port = 3456;                                       
  float cost = 10.123453e14;                                       

  int offset = 0;                                          

  memcpy(buf+offset, &s, sizeof(int));                     
  offset += sizeof(int);                                   

  memcpy(buf+offset, &port, sizeof(short));                
  offset += sizeof(short);                                 

  memcpy(buf+offset, &cost, sizeof(float));                
  offset += sizeof(float);                                 

  return offset;                                           
}                                                          

int main(int argc, char *argv[])
{
  int sock, buf_size, port;
  struct sockaddr_in dest_addr;
  int addr_size;
  unsigned char buf[1400];

  port = atoi(argv[1]);

  if((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
    perror("client: socket");
    return 1;
  }

  dest_addr.sin_family = AF_INET;
  dest_addr.sin_port = htons(port);
  dest_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  addr_size = sizeof(dest_addr);

  buf_size = construct_buffer(buf);

  if(sendto(sock,buf,buf_size,0,(struct sockaddr*)&dest_addr,addr_size) == -1) {
    close(sock);
    perror("client: sendto");
    return 1;
  }

  close(sock);

  return 0;
}
