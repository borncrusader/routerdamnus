#ifndef __DV_UDP_H__
#define __DV_UDP_H__

#include <map>

using namespace std;

#define MAX_LINE 256

struct table_key_t
{
  unsigned int addr;
  short port;
};

struct dv_table_value_t
{
  float cost;

  unsigned int nexthop_addr;
  short nexthop_port;
};

struct nb_table_value_t
{
  float cost;
};

struct nb_table_entry_t
{
  unsigned int addr;
  short port;

  float cost;
};

struct compare
{
  bool operator() (const table_key_t &a, const table_key_t &b) const {
    if(a.addr == b.addr) {
      return a.port < b.port;
    }
    return a.addr < b.addr;
  }
};

class dv_udp_t
{
  public:
  int sock;
  short port;

  bool initial;
  bool lo;

  map<table_key_t, dv_table_value_t, compare> dv_table;
  map<table_key_t, nb_table_value_t, compare> nb_table;

  void usage();
  void read_and_parse(const char *filename);
  void add_current_ip_to_dv_table();

  int dv_table_to_buf(unsigned char *buf);
  void buf_to_nb_vector(vector<nb_table_entry_t> &nb_vector, unsigned char *buf, int buf_size);

  int socket_handler();
};

#endif
