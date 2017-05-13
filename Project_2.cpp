#include<vector>
#include<iostream>
#include<string>
#include<set>
#include<stdlib.h>
using namespace std;

//returns 2^n
int bexp(int n) {
	int a = 1;
	for (int i = 0; i<n; i++) {
		a *= 2;
	}
	return a;
}

//returns a complete undirected adjacency matrix with random weights from 1-5 with "a" nodes
vector<vector<int>> matrix(int a) {
	vector<vector<int>> adjm(a, vector<int>(a));
	for (int i = 1; i < a; i++) {
		for (int j = 0; j < i; j++) {
			adjm[i][j] = rand() % 5 + 1;
			adjm[j][i] = test[i][j];
		}
	}
	for (int i = 0; i < a; i++) {
		adjm[i][i] = 0;
	}
	return adjm;
}

//returns the minimum distance from start to end, changes an integer reference 
//to how many hops there were, and increases an integer reference by how many steps were taken
//g is the adjacency matrix, size is how many nodes there are, start and end represent the nodes 
int dijkstra(const vector<vector<int>>& g, int size, int start, int end, int& hop, double& count) {
	//initialize distance and hops matrix for each node, starting from start
	vector<int> dis(size);
	vector<int> hops(size, 1);
	hops[start] = 0;
	dis = g[start];
	//create set of unvisited nodes
	set<int> unv;
	for (int i = 0; i < size; i++) {
		unv.insert(i);
	}
	unv.erase(start);
	//temporary variables
	int t1 = start;
	int t2;
	//while not all nodes are visited
	while (!unv.empty()) {
		//t2 starts at infinity
		t2 = 1000;
		//choose the closest unvisited node
		for (int i = 0; i < size; i++) {
			if (unv.find(i) != unv.end() && dis[i] < t2) {
				t2 = dis[i];
				t1 = i;
			}
		}
		//for all edges from that node, change all destinations if they are shorter
		//hops becomes the number of hops from the other place +1
		for (int i = 0; i < size; i++) {
			//increase the count for the number of steps
			count++;
			if (dis[i] > dis[t1] + g[t1][i]) {
				dis[i] = dis[t1] + g[t1][i];
				hops[i] = hops[t1] + 1;
			}
		}
		//mark node as visited
		unv.erase(t1);
	}
	//update the number of hops reference
	hop = hops[end];
	//return the shortest distance
	return dis[end];
}

//same thing as dijkstra's, but using bellman ford's algorithm
int bford(const vector<vector<int>>& g, int size, int start, int end, int& hop, double& count) {
	//initialize with start
	vector<int> dis(size);
	vector<int> hops(size, 1);
	dis = g[start];
	hops[start] = 0;
	//for all edges, if the destination's distance is greater than the start+distance, change it to 
	//start distance+distance
	//also add one to the number of hops from the start to destination
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (dis[j] > dis[i] + g[i][j]) {
				dis[j] = dis[i] + g[i][j];
				hops[j] = hops[i] + 1;
			}
			//increase the count
			count++;
		}
	}
	//update hops
	hop = hops[end];
	//return distance
	return dis[end];
}

//takes 1 argument to choose number of nodes, where number of nodes=2^input
int main(int argc, char** argv) {
	int n = stoi(argv[1]);
	n = bexp(n);
	//n is the number of nodes
	//for dijkstra, total is total distance, hotal is total hops, count is number of steps
	//for bellman ford, ttal2 is total distance, htal2 is total hops, count2 is number of steps
	float total = 0.0;
	float hotal = 0.0;
	float ttal2 = 0.0;
	float htal2 = 0.0;
	double count = 0;
	double count2 = 0;
	int temphop;
	//make 100 graphs with random start and end, sum up their results
	for (int i = 0; i < 100; i++) {
		vector<vector<int>> test = matrix(n);
		int start = rand() % n;
		int end = rand() % n;
		total += dijkstra(test, n, start, end, temphop, count);
		hotal += temphop;
		ttal2 += bford(test, n, start, end, temphop, count2);
		htal2 += temphop;
	}
	//divide by 100 for average
	total /= 100;
	hotal /= 100;
	ttal2 /= 100;
	htal2 /= 100;
	count /= 100;
	count2 /= 100;
	//cout input
	cout << "dijkstra average distance: " << total << endl;
	cout << "dijkstra average hops: " << hotal << endl;
	cout << "dijstra # of operations: " << count << endl;
	cout << "bellman ford average distance: " << ttal2 << endl;
	cout << "bellman ford average hops: " << htal2 << endl;
	cout << "bellman ford # of operations: " << count2 << endl;
	return 0;
}
