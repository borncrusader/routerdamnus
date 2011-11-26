#ifndef __ROUTERDAMNUS_H__
#define __ROUTERDAMNUS_H__

#include <vector>
#include <sys/time.h>

#define MAX_LINE 256

// nodes in file start from 1, represented from 0

struct adj_list_entry_t
{
  unsigned int vertex;                        // adjacent vertex's id
  float weight;                               // edge cost
};

struct queue_entry_t
{
  unsigned int vertex;                        // current node's vertex
  float path_cost;                            // current node's path cost
};

class router_t
{
  public:
  // member variables
  unsigned int num_v;
  std::vector < std::vector <adj_list_entry_t> > adj_list;

  timeval start_time;
  timeval end_time;

  // constructor and destructor
  router_t();
  ~router_t();

  // helper functions
  //void warn(const char *msg, int err);
  //void die(const char *msg, int err);

  void read_and_parse(const char *filename);
  int insert_adj_list(unsigned int v1, unsigned int v2, float weight);
  int read_adj_list();

  void set_start_time();
  void set_end_time();
  double get_diff_time();
  double get_cpu_time();
};

#endif
