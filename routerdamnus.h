#ifndef __ROUTERDAMNUS_H__
#define __ROUTERDAMNUS_H__

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cstdlib>
#include <limits>
#include <cerrno>
#include <cstring>

using namespace std;

#define MAX_LINE 256

struct adj_list_entry_t
{
  unsigned int vertex;
  float weight;
};

struct set_entry_t
{
  unsigned int vertex;
  float path_cost;
};

class router_t
{
  public:
  // member variables
  unsigned int num_v;
  vector < vector <adj_list_entry_t> > adj_list;

  // constructor and destructor
  router_t();
  ~router_t();

  // driver functions
  int link_state(unsigned int v1, unsigned int v2);
  //int distance_vector(void);

  // helper functions
  //void warn(const char *msg, int err);
  //void die(const char *msg, int err);

  void link_state_usage();
  void distance_vector_usage();
  void read_and_parse(const char *filename);
  int insert_adj_list(unsigned int v1, unsigned int v2, float weight);
  int read_adj_list();
};

#endif
