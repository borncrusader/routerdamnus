#include <iostream>
#include <cstdlib>
#include "routerdamnus.h"
#include "distance_vector.h"

using namespace std;

distance_vector_t::distance_vector_t()
{
}

distance_vector_t::~distance_vector_t()
{
}

void distance_vector_t::distance_vector_usage()
{
  cout<<"distance_vector initial-node file-name node1 node2"<<endl;
  exit(1);
}

int main(int argc, char *argv[])
{
  router_t router;
  unsigned int v1, v2;

  if(argc != 4) {
    distance_vector_t::distance_vector_usage();
  }

  router.read_and_parse(argv[1]);

  v1 = atoi(argv[2]);
  v2 = atoi(argv[3]);

  if(v1 > router.num_v || v2 > router.num_v) {
    distance_vector_t::distance_vector_usage();
  }

  return 0;
}
