all: link_state distance_vector dv_udp

link_state:
	g++ -g -lrt link_state.cpp routerdamnus.cpp -o link_state

distance_vector:
	g++ -g distance_vector.cpp routerdamnus.cpp -o distance_vector

dv_udp:
	g++ -g dv_udp.cpp -o dv_udp

clean:
	rm -rf link_state distance_vector dv_udp
