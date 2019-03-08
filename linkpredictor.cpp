#include <iostream>
#include "ActorGraph.hpp"
#include "ActorNode.hpp"
#include "MovieNode.hpp"
#include "MatrixMultiply.hpp"
#include <queue>
#include <fstream>
#include <sstream>
#include <utility>
#include <string>
#include <ctime>
#include <tuple>
using namespace std;

typedef tuple<int, int, string> mytuple;
class compare_mytuple
{
    public:
	bool operator()(mytuple &left, mytuple &right) const {
	    if(get<0>(left) != get<0>(right)) {
	        return get<0>(left) < get<0>(right);
	    }
	    else {
		return get<2>(left) > get<2>(right);
	    }			
	}
	
};
int main(int argc, const char** argv) {
    const char* inFileName;
    const char* ActorsFileName;
    const char* outFileName1;
    const char* outFileName2;
    if(argc != 5) {
	cout << "Incorrect number of arguments." << endl;
	exit(-1);
    }
    
    //bool have_header = false;
    //ifstream infile(ActorsFileName);
    inFileName = argv[1];
    ActorsFileName = argv[2];
    outFileName1 = argv[3];
    outFileName2 = argv[4];
    vector<string> input;
    ifstream in(ActorsFileName);
    string d_str;
    string str;
    getline(in, d_str);
    while(getline(in, str)) {
	if(str.size()>0) {
	    input.push_back(str);
	}
    }
    in.close();
    ofstream ofs1(outFileName1);
    ofstream ofs2(outFileName2);
    ofs1 << "Actor1,Actor2,Actor3,Actor4" << endl;
    ofs2 << "Actor1,Actor2,Actor3,Actor4" << endl;
    ActorGraph graph;
    graph.loadFromFile(inFileName, false);
    vector<vector<int>> new_adj(input.size(), vector<int>(graph.all_actor.size(), 0));
    for(int i=0; i<new_adj.size(); i++) {
    	unordered_map<string, ActorNode*>::const_iterator actor_itr = graph.all_actor.find(input[i]);
	new_adj[i] = graph.adj_matrix[actor_itr->second->index];
    }
    MatrixOperations<int> m1 = MatrixOperations<int>(new_adj, graph.adj_matrix);
    vector<vector<int>> prod_adj = m1.matrixMultiply();
    
    for(int j=0; j<prod_adj.size(); j++) {
	priority_queue<mytuple, vector<mytuple>, compare_mytuple> vector_q;
	for(int k=0; k<prod_adj[j].size(); k++) {
	    auto itr = graph.index_actor.find(k);
	    string name = itr->second;
	    
	    vector_q.push(make_tuple(prod_adj[j][k], k, name));
	}
	int master_index = get<1>(vector_q.top());
	int connected = 0;
	int unconnected = 0;
	vector_q.pop();
	while(connected!=4 || unconnected!=4)
	{
	    if(vector_q.empty()) {
		break;
	    }
	    int idx = get<1>(vector_q.top());
	    if(graph.adj_matrix[master_index][idx]==1 && connected!=4) {
		ofs1 << get<2>(vector_q.top()) << "\t";
		connected++;
	    }
	    else if(graph.adj_matrix[master_index][idx]==0 && unconnected!=4) {
		ofs2 << get<2>(vector_q.top()) << "\t";
		unconnected++;
	    }
	    vector_q.pop();	 	    
	}
	ofs1 << endl;
	ofs2 << endl;
    }
    ofs1.close();
    ofs2.close();

}
