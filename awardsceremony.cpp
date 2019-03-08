#include <iostream>
#include "ActorGraph.hpp"
#include "ActorNode.hpp"
#include "MovieNode.hpp"
#include <queue>
#include <fstream>
#include <sstream>
#include <utility>
#include <string>
using namespace std;
class compare_node
{
    public:
	bool operator()(pair<int, ActorNode*>& left, pair<int, ActorNode*>& right) const {
	    if(left.first != right.first) {
	        return left.first > right.first;
	    }
        }	
	
};
int main (int argc, const char** argv) {
    const char* inFileName;
    const char* outFileName;
    if(argc != 4) {
	cout << "Incorrect number of arguments." << endl;
	exit(-1);
    }
    inFileName = argv[1];
    int k = atoi(argv[2]);
    outFileName = argv[3];
    ofstream ofs(outFileName);
    ofs << "Actor" << endl;
    ActorGraph F;
    F.loadFromFile(inFileName, false);
    priority_queue<pair<int, ActorNode*>, vector<pair<int, ActorNode*>>, compare_node> list_q;
    
    for(auto a: F.all_actor) {
	list_q.push(make_pair(a.second->degree, a.second));
    }
    while(list_q.top().first < k) {
	if(list_q.top().second->visited == true) {
	    list_q.pop();
	}		
	else {
	    ActorNode* curr_actor = list_q.top().second;	    
	    list_q.top().second->visited = 1; 
	    list_q.pop();
	    for(int i=0; i<F.adj_matrix[curr_actor->index].size(); i++) {
	        if(F.adj_matrix[curr_actor->index][i] == 1) {
		    auto itr = F.index_actor.find(i);
		    auto itr_2 = F.all_actor.find(itr->second);
		    if(itr_2->second->visited != 1) {
			--(itr_2->second->degree);
		    	list_q.push(make_pair(itr_2->second->degree, itr_2->second)); 
		    }		
	        }
	    }
	}
    }
    for(int i=0; i<F.index_actor.size(); i++) {
	auto itr = F.index_actor.find(i);
	auto itr_2 = F.all_actor.find(itr->second);
	if(itr_2->second->visited == 0) {
	    ofs << itr->second << endl;	
	}
    }
    ofs.close();
}
