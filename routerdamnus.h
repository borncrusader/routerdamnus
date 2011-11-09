#ifndef __ROUTERDAMNUS_H__
#define __ROUTERDAMNUS_H__

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <limits>

using namespace std;

#define MAX_LINE 256

struct adj_list_entry_t
{
  unsigned int vertex;
  float weight;
};

struct queue_entry_t
{
  unsigned int vertex;
  unsigned int parent;
  float path_cost;
};

class compare_queue
{ 
  public:
    bool operator()(queue_entry_t, queue_entry_t);
};

class router_t
{
  public:
    unsigned int num_v;
    vector < vector <adj_list_entry_t> > adj_list;

    router_t();
    void usage(void);
    int read_and_parse(char *);
    int insert_adj_list(unsigned int, unsigned int, float);
    int read_adj_list();

    int link_state(unsigned int, unsigned int);
};

#endif
