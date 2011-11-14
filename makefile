all: link_state

link_state:
	g++ -g link_state.cpp routerdamnus.cpp -o link_state

clean:
	rm -rf link_state
