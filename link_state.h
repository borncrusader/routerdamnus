#ifndef __LINK_STATE_H__
#define __LINK_STATE_H__

#include <vector>

class link_state_t
{
  public:
  // member variables
  int vertex;

  std::vector<float> path_cost;
  std::vector<unsigned int> next_hop;

  // constructor and destructor
  link_state_t(int,int);
  ~link_state_t();

  // driver functions
  int compute_link_state(router_t&);

  static void link_state_usage();
};

#endif
