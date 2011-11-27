#!/usr/bin/env python

import sys, subprocess, time

# automate_test.py <file> <initial-node>
if len(sys.argv) < 3:
  print "automate_test.py <file> <initial-node>"
  sys.exit(0)

# read file to get adj list
f = open(sys.argv[1], 'r')
num_v = int(f.readline())
adj_list = [[] for i in range(0,num_v)]
for s in f:
  [v1,v2,w] = s.split(' ')
  [v1,v2,w] = [int(v1)-1,int(v2)-1,float(w)]
  adj_list[v1].append([v2,w])
  adj_list[v2].append([v1,w])
f.close()

init_v = int(sys.argv[2])-1

# construct the programs
for i in range(0,num_v):
  if init_v != i:
    nb=""
    for j in adj_list[i]:
      nb+="127.0.0.1 {0} {1} ".format(8001+j[0],j[1])

    cmd="dv_udp {0} {1} {2} > test_log_{3} &".format(0,8001+i,nb,i+1)
    if len(sys.argv)== 3:
      time.sleep(1)
      subprocess.call([cmd],shell=True)
    print cmd

# initial node
nb=""
for i in adj_list[init_v]:
  nb+="127.0.0.1 {0} {1} ".format(8001+i[0],i[1])

cmd="dv_udp {0} {1} {2} > test_log_{3} &".format(1,8001+init_v,nb,init_v+1)
if len(sys.argv) == 3:
  time.sleep(1)
  subprocess.call([cmd],shell=True)
print cmd
