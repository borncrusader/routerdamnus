#include <iostream>
#include <cstdlib>
#include <limits>
#include "routerdamnus.h"
#include "distance_vector.h"

using namespace std;

distance_vector_t::distance_vector_t(router_t& router)
{
  int i = 0, j = 0, k = 0;
  float max;
  adj_list_entry_t entry;

  max = numeric_limits<float>::max();

  B.resize(router.num_v, vector<bool>(router.num_v, false));

  for(i = 0 ; i < router.num_v ; i++) {
    D.push_back(vector< vector <float> >());
    for(j = 0 ; j < router.num_v ; j++) {
      D[i].push_back(vector<float>());
      for(k = 0 ; k < router.num_v ; k++) {
        if(i==j && j==k)
          D[i][j].push_back(0);
        else
          D[i][j].push_back(max);
      }
    }
  }

  for(i = 0 ; i < router.num_v ; i++) {
    for(j = 0 ; j < router.adj_list[i].size() ; j++) {
      entry.vertex = router.adj_list[i][j].vertex;
      entry.weight = router.adj_list[i][j].weight;
      D[i][i][entry.vertex] = entry.weight;
    }
  }

  /*for(i = 0 ; i < router.num_v ; i++)
    for(j = 0 ; j < router.num_v ; j++)
      for(k = 0 ; k < router.num_v ; k++)
        cout<<"i,j,k "<<i<<","<<j<<","<<k<<" D[][][]= "<<D[i][j][k]<<endl;*/
}

distance_vector_t::~distance_vector_t()
{
}

int distance_vector_t::send_dv_neighbors(router_t& router, unsigned int v) {
  int i = 0, j = 0, k = 0;

  for(i = 0 ; i < router.adj_list[v].size() ; i++) {
    k = router.adj_list[v][i].vertex;
    for(j = 0 ; j < router.num_v ; j++) {
      D[k][v][j] = D[v][v][j];
    }
    B[k][v] = true;
  }
}

int distance_vector_t::compute_distance_vector(router_t& router, unsigned int v)
{
  int i = 0, j = 0, k = 0;          // Loop Variables
  int num_iterations = 0;
  bool change = false, modify = false;

  send_dv_neighbors(router, v);

  while(true) {
    change = false;
    ++num_iterations;
    vector <int> s;
    for(i = 0 ; i < router.num_v ; i++) {
      modify = false;
      for(j = 0 ; j < router.num_v ; j++) {
        if(B[i][j] == false) {
          continue;
        }
        change = true;
        B[i][j] = false;
        for(k = 0 ; k < router.num_v ; k++) {
          if(D[i][i][k] > D[i][i][j] + D[i][j][k]) {
            modify = true;
            D[i][i][k] = D[i][i][j] + D[i][j][k];
          }
        }
      }
      if(modify) {
        s.push_back(i);
      }
    }
    for(i = 0 ; i < s.size() ; i++) {
      send_dv_neighbors(router, i);
    }
    if(change == false)
      break;
  }
}

void distance_vector_t::distance_vector_usage()
{
  cout<<"distance_vector initial-node file-name node1 node2"<<endl;
  exit(1);
}

int main(int argc, char *argv[])
{
  router_t router;
  unsigned int v1, v2, v;

  if(argc != 5) {
    distance_vector_t::distance_vector_usage();
  }

  v = atoi(argv[1]);
  router.read_and_parse(argv[2]);

  v1 = atoi(argv[3]);
  v2 = atoi(argv[4]);

  if(v1 > router.num_v || v2 > router.num_v) {
    distance_vector_t::distance_vector_usage();
  }

  distance_vector_t dv(router);

  dv.compute_distance_vector(router, v-1);

  return 0;
}
