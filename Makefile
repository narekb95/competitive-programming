run: out
	./out < in.txt

out: sol.cpp
	g++ -std=c++17 -O2 -Wall -Werror sol.cpp -o out


solution:
	cp algs/template.cpp sol.cpp
mst:
	cp algs/mst.cpp sol.cpp

dijkstra:
	cp algs/dijkstra.cpp sol.cpp

treap:
	cp algs/treap.cpp sol.cpp

unionfind:
	cp algs/unionfind.cpp sol.cpp

lca:
	cp algs/lca.cpp sol.cpp

clean:
	rm -f out sol
clean-all:
	rm -f sol.cpp out sol in.txt