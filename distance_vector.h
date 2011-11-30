#ifndef __DISTANCE_VECTOR_H__
#define __DISTANCE_VECTOR_H__

#include <vector>

using namespace std;

class distance_vector_t
{
  public:
  // member variables
  vector < vector < vector<float> > > D; // DV Table for each node
  vector < vector <unsigned int> > N; // Neighbor Array
  vector < vector<bool> > B; // Flag array
  vector <int> M;           // Track the number of iterations 
  vector <bool> I;          // Tracks the iteration 0

  // constructor and destructor
  distance_vector_t(router_t&, bool);
  ~distance_vector_t();

  // driver functions
  int send_dv_neighbors(router_t&, unsigned int);
  int compute_distance_vector(router_t&, unsigned int);
  int compute_distance_vector_r(router_t&);
  void print_distance_vector(router_t&, unsigned int, unsigned int);
  void print_all_distance_vector_table(router_t&);
  void print_all_distance_vector(router_t&);
  void print_adj_list(router_t&);

  static void distance_vector_usage();
};

#endif
