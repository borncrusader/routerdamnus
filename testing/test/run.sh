#!/bin/bash

for i in {1..32}
do
  link_state large-net-a.txt $i 10 >> ls_large_a_op
  link_state large-net-b.txt $i 10 >> ls_large_b_op
done
