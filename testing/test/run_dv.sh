#!/bin/bash

for i in {1..10}
do
  ./distance_vector -r small-net.txt $i 10 >> dv_small_op_r
  ./distance_vector $i small-net.txt $i 10 >> dv_small_op
done

for i in {1..32}
do
  ./distance_vector -r large-net-a.txt $i 20 >> dv_large_a_op_r
  ./distance_vector -r large-net-b.txt $i 20 >> dv_large_b_op_r
  ./distance_vector $i large-net-a.txt $i 20 >> dv_large_a_op
  ./distance_vector $i large-net-b.txt $i 20 >> dv_large_b_op
done
