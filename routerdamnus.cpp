#include <iostream>
#include <string>
#include <fstream>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <sys/resource.h>

#include "routerdamnus.h"

using namespace std;

router_t::router_t()
{
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
  unsigned int v1, v2;
  float e;
  char line[MAX_LINE];
  char *s;
  ifstream is;

  is.open(filename, ios::in);

  if(is.is_open() == false) {
    die("file could not be read!", errno);
  }

  is.getline(line, MAX_LINE);
  this->num_v = atoi(line);
  //printf("%d\n", this->num_v);

  this->adj_list.resize(this->num_v);
  while(!is.eof()) {
    is.getline(line, MAX_LINE);

    if(line == NULL)
      cout<<"NULL line"<<endl;
    s = strtok(line, " ");
    if(s == NULL) {
      cout<<"breaking"<<endl;
      break;
    }
    v1 = atoi(s);

    s = strtok(NULL, " ");
    v2 = atoi(s);

    s = strtok(NULL, " ");
    e = atof(s);

    printf("v1 : %d, v2 : %d, e : %f\n", v1, v2, e);
    this->insert_adj_list(v1-1, v2-1, e);
    this->insert_adj_list(v2-1, v1-1, e);
  }

  //this->read_adj_list();
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

void router_t::set_start_time()
{
  gettimeofday(&(this->start_time), NULL);
}

void router_t::set_end_time()
{
  gettimeofday(&(this->end_time), NULL);
}

double router_t::get_diff_time()
{
  return (this->end_time.tv_sec*1000000+this->end_time.tv_usec)-
    (this->start_time.tv_sec*1000000+this->start_time.tv_usec);
}

double router_t::get_cpu_time()
{
  rusage usage;

  getrusage(RUSAGE_SELF, &usage);

  cout<<usage.ru_utime.tv_sec<<"."<<usage.ru_utime.tv_usec<<endl;
  cout<<usage.ru_stime.tv_sec<<"."<<usage.ru_stime.tv_usec<<endl;
}
