#ifndef __DISTANCE_VECTOR_H__
#define __DISTANCE_VECTOR_H__

#include <vector>

using namespace std;

class distance_vector_t
{
  public:
  // member variables
  vector < vector < vector<float> > > D;
  vector < vector<bool> > B;

  // constructor and destructor
  distance_vector_t(router_t&);
  ~distance_vector_t();

  // driver functions
  int send_dv_neighbors(router_t&, unsigned int);
  int compute_distance_vector(router_t&, unsigned int);

  static void distance_vector_usage();
};

#endif
