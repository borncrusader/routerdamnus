#include <iostream>
#include <cstdlib>
#include <cstring>
#include <limits>
#include "routerdamnus.h"
#include "distance_vector.h"

using namespace std;

distance_vector_t::distance_vector_t(router_t& router, bool flag)
{
  int i = 0, j = 0, k = 0;
  float max;
  adj_list_entry_t entry;

  max = numeric_limits<float>::max();

  N.resize(router.num_v, vector<unsigned int>(router.num_v, 0));
  B.resize(router.num_v, vector<bool>(router.num_v, false));
  M.resize(router.num_v, 0);

  if(flag == true) {
    rflag = true;
  }
  else {
    rflag = false;
    I.resize(router.num_v, true);
  }

  for(i = 0 ; i < router.num_v ; i++) {
    D.push_back(vector< vector <float> >());
    for(j = 0 ; j < router.num_v ; j++) {
      D[i].push_back(vector<float>());
      for(k = 0 ; k < router.num_v ; k++) {
        if(i==j && j==k) {
          D[i][j].push_back(0);
          N[i][j] = i;
        }
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
      N[i][entry.vertex] = entry.vertex;
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
  int max = numeric_limits<float>::max();

  if(rflag) {
    for(i = 0 ; i < router.num_v ; i++)
      send_dv_neighbors(router, i);
  }
  else {
    send_dv_neighbors(router, v);
  }

  while(true) {
    change = false;
    //++num_iterations;
    vector <int> s;
    for(i = 0 ; i < router.num_v ; i++) {
      modify = false;
      //cout<<"\nNode : "<<i+1;
      for(j = 0 ; j < router.num_v ; j++) {
        if(B[i][j] == false) {
          continue;
        }
        //cout<<"NAN="<<j+1<<" ";
        change = true;
        B[i][j] = false;
        //cout<<"\tReceived New DV From : "<<j+1<<endl;
        for(k = 0 ; k < router.num_v ; k++) {
          if(D[i][i][k] > D[i][i][j] + D[i][j][k]) {
            modify = true;
            D[i][i][k] = D[i][i][j] + D[i][j][k];
            N[i][k] = j;
            //cout<<"\t\tChanging distance to : "<<k+1<<endl;
          }
        }
        //cout<<endl;
      }
      if(!rflag && (modify || I[i])) {
        s.push_back(i);
        I[i] = false;
        ++M[i];
      }
      else if(rflag && modify) {
        s.push_back(i);
        ++M[i];
      }
    }

    for(i = 0 ; i < s.size() ; i++) {
      send_dv_neighbors(router,s[i]);
    }
    if(change == false)
      break;
  }

  for(i = 0 ; i < router.num_v ; i++) {
    if(M[i] > num_iterations) {
      num_iterations = M[i];
    }
  }

  cout<<"\nNumber of Iterations to Converge : "<<num_iterations<<endl<<endl;
}

void distance_vector_t::print_distance_vector(router_t& router, unsigned int v1, unsigned int v2)
{
  int i = 0;

  cout<<"Routing Table of Node "<<v1+1<<endl;
  cout<<"------------------------\n";
  cout<<"Node\tCost\tNext Hop\n";
  for(i = 0 ; i < router.num_v ; i++) {
    cout<<i+1<<"\t"<<D[v1][v1][i]<<"\t"<<N[v1][i]+1<<endl;
  }
  cout<<endl;

  cout<<"Routing Table of Node "<<v2+1<<endl;
  cout<<"------------------------\n";
  cout<<"Node\tCost\tNext Hop\n";
  for(i = 0 ; i < router.num_v ; i++) {
    cout<<i+1<<"\t"<<D[v2][v2][i]<<"\t"<<N[v2][i]+1<<endl;
  }
  cout<<endl;
}

void distance_vector_t::print_all_distance_vector(router_t& router)
{
  int i = 0, j = 0;
  for(i = 0 ; i < router.num_v ; i++) {
    cout<<"DV of Node "<<i+1<<endl;
    for(j = 0 ; j < router.num_v ; j++) {
      cout<<D[i][i][j]<<"  ";
    }
    cout<<endl;
  }
}

void distance_vector_t::print_all_distance_vector_table(router_t& router)
{
  int i = 0, j = 0, k = 0;
  int max = numeric_limits<float>::max();
  for(i = 0 ; i < router.num_v ; i++) {
    cout<<"DV of Node "<<i+1<<endl;
    for(j = 0 ; j < router.num_v ; j++) {
      for(k = 0 ; k < router.num_v ; k++) {
        cout<<D[i][j][k]<<"  ";
      }
      cout<<endl;
    }
    cout<<endl;
  }
}

void distance_vector_t::print_adj_list(router_t& router)
{
  int i = 0, j = 0;
  for(i = 0 ; i < router.num_v ; i++) {
    cout<<i+1<<":";
    for(j = 0 ; j < router.adj_list[i].size() ; j++) {
      cout<<router.adj_list[i][j].vertex<<",";
    }
    cout<<endl;
  }
}

void distance_vector_t::distance_vector_usage()
{
  cout<<"distance_vector [initial-node]|[-r] file-name node1 node2"<<endl;
  exit(1);
}

int main(int argc, char *argv[])
{
  bool flag = false;
  router_t router;
  unsigned int v1, v2, v;

  if(argc != 5) {
    distance_vector_t::distance_vector_usage();
  }

  if(!strcmp(argv[1], "-r"))
    flag = true;
  else
    v = atoi(argv[1]);

  router.read_and_parse(argv[2]);

  v1 = atoi(argv[3]);
  v2 = atoi(argv[4]);

  if(v1 > router.num_v || v2 > router.num_v) {
    distance_vector_t::distance_vector_usage();
  }

  distance_vector_t dv(router, flag);

  //dv.print_adj_list(router);

  dv.compute_distance_vector(router, v-1);

  cout<<"Cost from "<<v1<<" to "<<v2<<" : "<<dv.D[v1-1][v1-1][v2-1]<<endl<<endl;

  dv.print_distance_vector(router, v1-1, v2-1);

  //dv.print_all_distance_vector_table(router);

  //dv.print_all_distance_vector(router);

  return 0;
}
