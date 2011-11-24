#include <iostream>
#include <set>
#include <limits>
#include <cstdlib>
#include "routerdamnus.h"
#include "link_state.h"

using namespace std;

link_state_t::link_state_t(int vertex, int num_v)
{
  this->vertex = vertex;

  this->path_cost.reserve(num_v);
  this->parent.reserve(num_v);
}

link_state_t::~link_state_t()
{
}

void link_state_t::link_state_usage()
{
  cout<<"link_state file-name node1 node2"<<endl;
  exit(1);
}

struct compare
{
  bool operator()(const queue_entry_t& m, const queue_entry_t& n) const {
    return m.path_cost <= n.path_cost;
  }
};

int link_state_t::compute_link_state(router_t& router)
{
  set<queue_entry_t,compare> queue;
  queue_entry_t top, entry;

  float max_float = numeric_limits<float>::max();
  unsigned int i, vertex;

  for(i=0; i<router.num_v; i++) {
    // start with parent as it's own and cost as max_float, except the source
    this->parent[i] = i;
    if(i == this->vertex) {
      this->path_cost[i] = 0;
    } else {
      this->path_cost[i] = max_float;
    }
    entry.vertex = i;
    entry.path_cost = this->path_cost[i];
    queue.insert(entry);
  }

  while(queue.empty() == false) {
    // begin will get the smallest element
    top = *queue.begin();
    cout<<"removing : "<<top.vertex+1<<","<<top.path_cost<<endl;

    for(i=0; i<router.adj_list[top.vertex].size(); i++) {
      // relax edges in the adj list
      vertex = router.adj_list[top.vertex][i].vertex;

      if(top.path_cost + router.adj_list[top.vertex][i].weight < this->path_cost[vertex]) {
        cout<<"relaxing : "<<vertex<<","<<this->path_cost[vertex]<<" with "<<top.path_cost + router.adj_list[top.vertex][i].weight<<endl;

        entry.vertex = vertex;

        entry.path_cost = this->path_cost[vertex];
        queue.erase(entry);

        entry.path_cost = top.path_cost + router.adj_list[top.vertex][i].weight;
        path_cost[vertex] = entry.path_cost;
        parent[vertex] = top.vertex;
          
        queue.insert(entry);
      }
    }

    queue.erase(queue.begin());
  }
}

int main(int argc, char *argv[])
{
  router_t router;
  unsigned int v1, v2;
  int i;

  if(argc != 4) {
    link_state_t::link_state_usage();
  }

  router.read_and_parse(argv[1]);

  v1 = atoi(argv[2]);
  v2 = atoi(argv[3]);

  if(v1 > router.num_v || v2 > router.num_v) {
    link_state_t::link_state_usage();
  }

  link_state_t l1(v1-1,router.num_v), l2(v2-1,router.num_v);

  l1.compute_link_state(router);
  l2.compute_link_state(router);

  for(i=0;i<router.num_v;i++) {
    cout<<i+1<<" : "<<l1.path_cost[i]<<endl;
  }

  return 0;
}
