#include <iostream>
#include <iomanip>
#include <fstream>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <vector>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <unistd.h>

#include "dv_udp.h"

void warn(const char *msg, int err)
{
  if(err== 0) {
    cout<<msg<<endl;
  } else {
    cout<<msg<<": "<<strerror(err)<<endl;
  }
}

void die(const char *msg, int err)
{
  warn(msg, err);
  exit(err);
}

void dv_udp_t::usage()
{
  cout<<"dv_udp [-f cfg-file] | [initial port nb-ip nb-port nb-cost ... ]"<<endl;
  cout<<"       config can be given through cfg-file or"<<endl;
  cout<<"       initial - should the node be an initial node?"<<endl;
  cout<<"       port - listening port of the node"<<endl;
  cout<<"       nb-ip - neighbour's ip"<<endl;
  cout<<"       nb-port - neighbour's port"<<endl;
  cout<<"       nb-cost - cost to neighbour"<<endl;
  exit(1);
}

void dv_udp_t::read_and_parse(const char *filename)
{
  char line[MAX_LINE], *s;
  ifstream is;
  table_key_t k;
  dv_table_value_t d;
  nb_table_value_t n;

  is.open(filename, ios::in);

  if(is.is_open() == false) {
    die("file could not be read!", errno);
  }

  is.getline(line, MAX_LINE);
  this->initial = (bool)atoi(line);

  this->lo = 0;

  is.getline(line, MAX_LINE);
  this->port = (short)atoi(line);

  while(true) {
    is.getline(line, MAX_LINE);
    if(is.eof()) {
      break;
    }

    s = strtok(line, " ");
    k.addr = inet_addr(s);

    if(atoi(s) == 127) {
      // know whether it is a loopback simulation
      this->lo = 1;
    }

    s = strtok(NULL, " ");
    k.port = (short)atoi(s);
  
    s = strtok(NULL, " ");
    d.cost = atof(s);

    d.nexthop_addr = k.addr;
    d.nexthop_port = k.port;
    this->dv_table[k] = d;
  
    n.cost = d.cost;
    this->nb_table[k] = n;

    //cout<<k.addr<<" "<<k.port<<" "<<d.cost<<endl;
  }

  is.close();

  return;
}

void dv_udp_t::add_current_ip_to_dv_table()
{
  table_key_t k;
  dv_table_value_t d;
  char host[NI_MAXHOST];

  ifaddrs *ifaddr, *ifa;

  if (getifaddrs(&ifaddr) == -1) {
    die("add_current_ip : getifaddrs failed! ", errno);
  }

  for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
    if(ifa->ifa_addr->sa_family == AF_INET) {
      if (getnameinfo(ifa->ifa_addr, sizeof(sockaddr_in), host, NI_MAXHOST,
              NULL, 0, NI_NUMERICHOST)) {
        die("add_current_ip : getnameinfo() failed ", errno);
      }

      k.addr = inet_addr(host);
      k.port = this->port;

      d.cost = 0;
      d.nexthop_addr = inet_addr(host);
      d.nexthop_port = this->port;

      if(this->lo && !strcmp(ifa->ifa_name, "lo")) {
        this->dv_table[k] = d;
        break;
      } else if(!this->lo && strcmp(ifa->ifa_name,"lo")) {
        this->dv_table[k] = d;
      }
    }
  }

  freeifaddrs(ifaddr);
  return;
}

int dv_udp_t::dv_table_to_buf(unsigned char *buf)
{
  int offset = 0, len = this->dv_table.size();
  map<table_key_t,dv_table_value_t>::iterator it;

  memcpy(buf+offset, &len, sizeof(int));
  offset += sizeof(int);

  for(it=this->dv_table.begin(); it!=this->dv_table.end(); it++) {
    memcpy(buf+offset, &((*it).first.addr), sizeof(int));
    offset += sizeof(int);

    memcpy(buf+offset, &((*it).first.port), sizeof(short));
    offset += sizeof(short);

    memcpy(buf+offset, &((*it).second.cost), sizeof(float));
    offset += sizeof(float);
  }

  return offset;
}

void dv_udp_t::buf_to_nb_vector(vector<nb_table_entry_t> &nb_vector, unsigned char *buf, int buf_size)
{
  int len, offset = 0;
  nb_table_entry_t e;

  memcpy(&len, buf+offset, sizeof(int));
  offset += sizeof(int);

  while(offset < buf_size) {
    memcpy(&(e.addr), buf+offset, sizeof(int));
    offset += sizeof(int);

    memcpy(&(e.port), buf+offset, sizeof(short));
    offset += sizeof(short);

    memcpy(&(e.cost), buf+offset, sizeof(float));
    offset += sizeof(float);

    nb_vector.push_back(e);
  }
}

int dv_udp_t::socket_handler()
{
  int i, buf_size;
  socklen_t addr_size = sizeof(sockaddr_in);
  sockaddr_in addr;
  in_addr display_addr;
  unsigned char buf[1400];
  vector<nb_table_entry_t> nb_vector;
  table_key_t key, neighbour;
  map<table_key_t, dv_table_value_t>::iterator dv_it;
  map<table_key_t, nb_table_value_t>::iterator nb_it;
  dv_table_value_t d_value;
  nb_table_value_t n_value;
  bool changed;
  int convergence = 0;

  if((this->sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
   die("dv_udp : socket creation failed!", errno);
  }

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(this->port);

  if(bind(this->sock, (sockaddr*)&addr, sizeof(addr)) == -1) {
    close(this->sock);
    die("dv_udp : bind failed!", errno);
  }

  if(this->initial) {
    // go on and send my dv to all neighbours
    buf_size = this->dv_table_to_buf(buf);

    for(nb_it = this->nb_table.begin(); nb_it!=this->nb_table.end(); nb_it++) {
      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = (*nb_it).first.addr;
      addr.sin_port = htons((*nb_it).first.port);

      if(sendto(this->sock,buf,buf_size,0,(sockaddr*)&addr,addr_size) == -1) {
        close(this->sock);
        die("dv_udp : sendto failed!", errno);
        return 1;
      }
    }
  }

  while(true) {
    changed = false;
    addr_size = sizeof(sockaddr_in);
    nb_vector.clear();

    buf_size = recvfrom(this->sock,buf,1400,0,(sockaddr*)&addr,&addr_size);
    this->buf_to_nb_vector(nb_vector, buf, buf_size);

    neighbour.addr = addr.sin_addr.s_addr;
    neighbour.port = ntohs(addr.sin_port);
    nb_it = this->nb_table.find(neighbour);

    //cout<<"received dv-vector from "<<inet_ntoa(addr.sin_addr)<<":"<<neighbour.port<<endl;

    if(nb_it == this->nb_table.end()) {
      // vector from unknown neighbour!!
      cout<<"howl!"<<" "<<this->nb_table.size()<<endl;
      cout<<neighbour.addr<<" "<<neighbour.port<<endl;
      map<table_key_t, nb_table_value_t>::iterator temp_it;
      for(temp_it=this->nb_table.begin(); temp_it!=this->nb_table.end(); temp_it++) {
        cout<<(*temp_it).first.addr<<" "<<(*temp_it).first.port<<endl;
      }
      continue;
    }

    for(i=0; i<nb_vector.size(); i++) {
      key.addr = nb_vector[i].addr;
      key.port = nb_vector[i].port;
      dv_it = this->dv_table.find(key);

      display_addr.s_addr = nb_vector[i].addr;
      //cout<<i+1<<" "<<inet_ntoa(display_addr)<<":"<<nb_vector[i].port<<" "<<nb_vector[i].cost<<endl;

      if(dv_it == this->dv_table.end()) {
        // add new entries to my dv table
        d_value.cost = nb_vector[i].cost + (*nb_it).second.cost;
        d_value.nexthop_addr = neighbour.addr;
        d_value.nexthop_port = neighbour.port;

        this->dv_table[key] = d_value;
        changed = true;
      } else {
        if(nb_vector[i].cost + (*nb_it).second.cost < (*dv_it).second.cost)  {
          // recompute_my_dv_table
          (*dv_it).second.cost = nb_vector[i].cost + (*nb_it).second.cost;
          (*dv_it).second.nexthop_addr = neighbour.addr;
          (*dv_it).second.nexthop_port = neighbour.port;
          changed = true;
        }
      }
    }

    // send my dv table
    buf_size = this->dv_table_to_buf(buf);

    for(nb_it = this->nb_table.begin(); nb_it!=this->nb_table.end(); nb_it++) {
      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = (*nb_it).first.addr;
      addr.sin_port = htons((*nb_it).first.port);

      if(sendto(this->sock,buf,buf_size,0,(sockaddr*)&addr,addr_size) == -1) {
        close(this->sock);
        die("dv_udp : sendto failed!", errno);
        return 1;
      }
    }

    if(changed == false) {
      convergence++;
      // convergence threshold = 5
      if(convergence > this->nb_table.size()) {
        //cout<<"converged!"<<endl;
        break;
      }
    } else {
      convergence = 0;
    }
  }

  cout<<"forwarding table"<<endl;
  cout<<setw(21)<<"destination"<<" ";
  cout<<setw(10)<<"cost"<<" ";
  cout<<setw(22)<<"next hop"<<" "<<endl;
  for(dv_it=this->dv_table.begin(); dv_it!=this->dv_table.end(); dv_it++) {
    display_addr.s_addr = (*dv_it).first.addr;
    cout<<setw(15)<<inet_ntoa(display_addr)<<":";
    cout<<setw(5)<<(*dv_it).first.port<<" ";
    cout<<setw(10)<<(*dv_it).second.cost<<" ";

    display_addr.s_addr = (*dv_it).second.nexthop_addr;

    cout<<setw(16)<<inet_ntoa(display_addr)<<":";
    cout<<setw(5)<<(*dv_it).second.nexthop_port<<endl;
  }
}

int main(int argc, char *argv[])
{
  dv_udp_t dv_udp;
  table_key_t k;
  dv_table_value_t d;
  nb_table_value_t n;
  int i;

  if(argc < 3 || argc%3 != 0) {
    dv_udp.usage();
  }

  if(!strcmp(argv[1], "-f")) {
    dv_udp.read_and_parse(argv[2]);
  } else {
    dv_udp.initial = (bool)atoi(argv[1]);
    dv_udp.port = (short)atoi(argv[2]);

    for(i=3; i<argc; i+=3) {
      k.addr = inet_addr(argv[i]);
      k.port = (short)atoi(argv[i+1]);
      d.cost = atof(argv[i+2]);
      d.nexthop_addr = k.addr;
      d.nexthop_port = k.port;
      dv_udp.dv_table[k] = d;

      n.cost = d.cost;
      dv_udp.nb_table[k] = n;

      if(atoi(argv[i]) == 127) {
        dv_udp.lo = 1;
      }
    }
  }

  dv_udp.add_current_ip_to_dv_table();

  dv_udp.socket_handler();

  return 0;
}
