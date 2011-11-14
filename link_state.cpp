#include "routerdamnus.h"

int main(int argc, char *argv[])
{
  router_t router;
  unsigned int v1, v2;

  if(argc != 4) {
    router.link_state_usage();
  }

  router.read_and_parse(argv[1]);

  v1 = atoi(argv[2]);
  v2 = atoi(argv[3]);

  router.link_state(v1, v2);

  return 0;
}
