#include "routerdamnus.h"

router_t::router_t()
{
  this->num_v = 0;
  return;
}

void router_t::usage(void)
{
  cout<<"routerdamnus <file>"<<endl;
  exit(1);
}

int router_t::read_and_parse(char *filename)
{
  int i, j;
  unsigned int v1, v2;
  float e;
  char line[MAX_LINE], c[MAX_LINE];
  ifstream is;

  is.open(filename, ios::in);

  if(is.is_open() == false) {
    // die
    cout<<"no file!"<<endl;
    return 1;
  }

  is.getline(line, MAX_LINE);
  this->num_v = atoi(line);
  printf("%d\n", this->num_v);

  this->adj_list.reserve(this->num_v);
  while(true) {
    is.getline(line, MAX_LINE);
    if(is.eof()) {
      cout<<"stop!"<<endl;
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

  return 0;
}

int router_t::insert_adj_list(unsigned int v1, unsigned int v2, float weight)
{
  adj_list_entry_t entry;

  entry.vertex = v2;
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
      printf("%d, e: %f; ", this->adj_list[i][j].vertex, this->adj_list[i][j].weight);
    }
    cout<<endl;
  }

  return 1;
}

bool compare_queue::operator()(queue_entry_t e1, queue_entry_t e2)
{
  if(e1.path_cost>e2.path_cost) {
    return true;
  }

  return false;
}

int router_t::link_state(unsigned int v1, unsigned int v2)
{
  priority_queue<queue_entry_t, vector<queue_entry_t>, compare_queue> q;
  queue_entry_t e;

  for(i=0; i<this->num_v; i++) {
    e.vertex = i+1; // this entry's vertex id
    e.parent = i+1; // each has it as its own parent at the start
    if(i == v1-1) {
      // for the start node, the path_cost is initialised as 0
      e.path_cost = 0;
    } else {
      // rest of the nodes, it is the max possible value
      e.path_cost = numeric_limits<float>::max();
    }

    q.push(e);
  }

  while(q.empty() == false) {
    e = q.top();

    for(i=0; i<this->adj_list[e.vertex-1].size(); i++) {

    }


    q.pop();
  }
  //   retrieve the first element from the queue
  //   check the adj list of this node and update its adjacent vertices
}

int main(int argc, char *argv[])
{
  router_t router;

  if(argc != 2) {
    router.usage();
  }

  if(router.read_and_parse(argv[1])) {
    return 1;
  }

  return 0;
}
