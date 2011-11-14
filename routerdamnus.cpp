#include "routerdamnus.h"

router_t::router_t()
{
  this->num_v = 0;
  return;
}

router_t::~router_t()
{
}

struct compare {
  bool operator()(const set_entry_t s1, const set_entry_t s2) {
    if(s1.path_cost < s2.path_cost) {
      return true;
    } else {
      return false;
    }

  }
};

int router_t::link_state(unsigned int v1, unsigned int v2)
{
  set <set_entry_t, compare> edge_set;
  set<set_entry_t>::iterator find_iter;
  set_entry_t e, f;
  int i;
  float path_cost[this->num_v];
  unsigned int parent[this->num_v];

  for(i=0; i<this->num_v; i++) {
    if(i == v1-1) {
      path_cost[i] = 0;
    } else {
      path_cost[i] = numeric_limits<float>::max();
    }
    parent[i] = i;
      /*
    e.vertex = i; // this entry's vertex id
    e.parent = i; // each has it as its own parent at the start
    if(i == v1-1) {
      // for the start node, the path_cost is initialised as 0
      e.path_cost = 0;
    } else {
      // rest of the nodes, it is the max possible value
      e.path_cost = numeric_limits<float>::max();
    }

    edge_set.insert(e);
    */
  }
  e.vertex = v1-1;
  e.path_cost = 0;
  edge_set.insert(e);

  while(edge_set.empty() == false) {
    e = *(edge_set.begin());

    for(i=0; i<this->adj_list[e.vertex].size(); i++) {
      if(e.path_cost + this->adj_list[e.vertex][i].weight < path_cost[i]) {
        f.vertex = e.vertex;
        f.path_cost = e.path_cost;

        find_iter = edge_set.find(f);
      }
    }

    edge_set.erase(edge_set.begin());
  }
  //   retrieve the first element from the queue
  //   check the adj list of this node and update its adjacent vertices
}

void warn(const char *msg, int err)
{
  if(err== 0) {
    cout<<msg<<endl;
  } else {
    cout<<msg<<": "<<strerror(err)<<endl;
  }
}

void die(const char *msg, int err)
{
  warn(msg, err);

  exit(err);
}

void router_t::link_state_usage()
{
  cout<<"link_state file-name node1 node2"<<endl;
  exit(1);
}

void router_t::distance_vector_usage()
{
  cout<<"distance_vector initial-node file-name node1 node2"<<endl;
  exit(1);
}

void router_t::read_and_parse(const char *filename)
{
  int i, j;
  unsigned int v1, v2;
  float e;
  char line[MAX_LINE], c[MAX_LINE];
  ifstream is;

  is.open(filename, ios::in);

  if(is.is_open() == false) {
    die("file could not be read!", errno);
  }

  is.getline(line, MAX_LINE);
  this->num_v = atoi(line);
  printf("%d\n", this->num_v);

  this->adj_list.reserve(this->num_v);
  while(true) {
    is.getline(line, MAX_LINE);
    if(is.eof()) {
      break;
    }

    for(i=0; line[i] >= '0' && line[i] <= '9'; i++) {
      c[i] = line[i];
    }
    c[i] = '\0';
    v1 = atoi(c);

    for(; line[i] < '0' || line[i] > '9'; i++);
    for(j=0; line[i] >= '0' && line[i] <= '9'; j++, i++) {
      c[j] = line[i];
    }
    c[j] = '\0';
    v2 = atoi(c);

    for(; (line[i] < '0' || line[i] > '9') && line[i] != '.'; i++);
    for(j=0; (line[i] >= '0' && line[i] <= '9') || line[i] == '.'; j++, i++) {
      c[j] = line[i];
    }
    c[j] = '\0';
    e = atof(c);

    printf("v1 : %d, v2 : %d, e : %f\n", v1, v2, e);
    this->insert_adj_list(v1, v2, e);
  }

  read_adj_list();
  is.close();

  return;
}

int router_t::insert_adj_list(unsigned int v1, unsigned int v2, float weight)
{
  adj_list_entry_t entry;

  entry.vertex = v2-1;
  entry.weight = weight;

  this->adj_list[v1-1].push_back(entry);

  return 1;
}

int router_t::read_adj_list()
{
  int i, j;

  for(i=0; i<this->num_v; i++) {
    printf("%d: ", i+1);
    for(j=0; j<this->adj_list[i].size(); j++) {
      printf("%d, e: %f; ", this->adj_list[i][j].vertex+1, this->adj_list[i][j].weight);
    }
    cout<<endl;
  }

  return 1;
}
