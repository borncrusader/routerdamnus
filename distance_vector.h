#ifndef __DISTANCE_VECTOR_H__
#define __DISTANCE_VECTOR_H__

class distance_vector_t
{
  public:
  // member variables

  // constructor and destructor
  distance_vector_t();
  ~distance_vector_t();

  // driver functions
  int compute_distance_vector(router_t&);

  static void distance_vector_usage();
};

#endif
