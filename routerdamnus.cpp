#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstring>
#include <cstdlib>

#include "routerdamnus.h"

using namespace std;

router_t::router_t()
{
  this->num_v = 0;
  return;
}

router_t::~router_t()
{
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

    //printf("v1 : %d, v2 : %d, e : %f\n", v1, v2, e);
    this->insert_adj_list(v1-1, v2-1, e);
    this->insert_adj_list(v2-1, v1-1, e);
  }

  this->read_adj_list();
  is.close();

  return;
}

int router_t::insert_adj_list(unsigned int v1, unsigned int v2, float weight)
{
  adj_list_entry_t entry;

  entry.vertex = v2;
  entry.weight = weight;

  this->adj_list[v1].push_back(entry);

  return 1;
}

int router_t::read_adj_list()
{
  int i, j;

  for(i=0; i<this->num_v; i++) {
    cout<<i+1<<":";
    for(j=0; j<this->adj_list[i].size(); j++) {
      cout<<" "<<this->adj_list[i][j].vertex+1<<","<<this->adj_list[i][j].weight<<";";
      //printf(" %d,%0.2f;", this->adj_list[i][j].vertex+1, this->adj_list[i][j].weight);
    }
    cout<<endl;
  }

  return 1;
}
