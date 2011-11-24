all: link_state distance_vector

link_state:
	g++ -g link_state.cpp routerdamnus.cpp -o link_state

distance_vector:
	g++ -g distance_vector.cpp routerdamnus.cpp -o distance_vector

clean:
	rm -rf link_state distance_vector
