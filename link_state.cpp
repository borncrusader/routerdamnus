#include <iostream>
#include <iomanip>
#include <set>
#include <limits>
#include <cstdlib>
#include "routerdamnus.h"
#include "link_state.h"

using namespace std;

link_state_t::link_state_t(int vertex, int num_v)
{
  this->vertex = vertex;

  this->path_cost.resize(num_v);
  this->next_hop.resize(num_v);
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
  unsigned int i, v;

  for(i=0; i<router.num_v; i++) {
    // start with next hop as its own and cost as max_float, except the source
    this->next_hop[i] = i;
    if(i == this->vertex) {
      this->path_cost[i] = 0;
    } else {
      this->path_cost[i] = max_float;
    }
    entry.vertex = i;
    entry.path_cost = this->path_cost[i];
    queue.insert(entry);
    //cout<<queue.size()<<endl;
  }

  while(queue.empty() == false) {
    // begin will get the smallest element
    top = *queue.begin();
    //cout<<"removing : "<<top.vertex+1<<","<<top.path_cost<<endl;

    for(i=0; i<router.adj_list[top.vertex].size(); i++) {
      // relax edges in the adj list
      v = router.adj_list[top.vertex][i].vertex;

      if(top.path_cost + router.adj_list[top.vertex][i].weight < this->path_cost[v]) {

        //cout<<"relaxing : "<<v<<","<<this->path_cost[v]<<" with "<<top.path_cost + router.adj_list[top.vertex][i].weight<<endl;

        entry.vertex = v;

        entry.path_cost = this->path_cost[v];
        queue.erase(entry);

        entry.path_cost = top.path_cost + router.adj_list[top.vertex][i].weight;
        this->path_cost[v] = entry.path_cost;

        if(top.vertex == this->vertex) {
          // for the source node removed from the set,
          // set the next hop as the next node
          this->next_hop[v] = v;
        } else {
          // for the other nodes, the next hop will be the
          // next hop of the parent node
          this->next_hop[v] = this->next_hop[top.vertex];
        }

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

  router.set_start_time();
  l1.compute_link_state(router);
  router.set_end_time();
  l2.compute_link_state(router);

  cout<<"cost from "<<v1<<" to "<<v2<<" : "<<l1.path_cost[v2-1]<<endl;

  cout<<endl<<"routing table of "<<v1<<endl;
  cout<<"   node   cost   next hop"<<endl;
  for(i=0;i<router.num_v;i++) {
    cout<<setw(7)<<i+1<<"\t"<<setw(6)<<l1.path_cost[i]<<setw(11)<<l1.next_hop[i]+1<<endl;
  }

  cout<<endl<<"routing table of "<<v2<<endl;
  cout<<"   node   cost   next hop"<<endl;
  for(i=0;i<router.num_v;i++) {
    cout<<setw(7)<<i+1<<"\t"<<setw(6)<<l2.path_cost[i]<<setw(11)<<l2.next_hop[i]+1<<endl;
  }

  cout<<"time to run : "<<router.get_diff_time()<<" us"<<endl;

  return 0;
}
