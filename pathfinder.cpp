#include <iostream>
#include "ActorGraph.hpp"
#include "ActorNode.hpp"
#include "MovieNode.hpp"
#include <queue>
#include <fstream>
#include <sstream>
#include <utility>
#include <string.h>
using namespace std;

void BFS(ActorNode* actor_1, ActorNode* actor_2);
void Djikstra(ActorNode* actor_1, ActorNode* actor_2);
vector< pair<string, string> > loadInputFile(const char* inFileName);
void print_path(ActorNode* last_actor, ofstream& ofs); 
int main(int argc, const char** argv) 
{
    bool weightedGraph = false; 
    const char* inFileName;
    const char* twoActorsFileName;
    const char* outFileName;
    vector< pair<string, string> > twoActors;
    //cout << "Checkpoint 1" << endl;
    if(argc != 5) {
	cout << "Incorrect number of arguments." << endl;
	exit(-1);
    }
    
    //cout << "Checkpoint 2" << endl;
    inFileName = argv[1];
    twoActorsFileName = argv[3];
    outFileName = argv[4];
 
    //cout << "Checkpoint 3" << endl;
    if(strcmp(argv[2],"u") == 0 ) {
    	weightedGraph = false;
    }
    else if(strcmp(argv[2], "w") == 0) {
	weightedGraph = true;
    }
    else {		
	cout << "Incorrect second argument. Should be 'u' or 'w'" << endl;
	exit(-1);
    }
    //cout << "Checkpoint 4" << endl;
    ActorGraph graph;
    graph.loadFromFile(inFileName, weightedGraph);
    cout << graph.all_actor.size() << endl;
    /*for(auto a: graph.index_actor) {
    	cout << a.first << " " << a.second << endl;
    }*/

    twoActors = loadInputFile(twoActorsFileName);
    ofstream ofs(outFileName);
    ofs << "(actor)--[movie#@year]-->(actor)--..." << endl;
    for(auto a: twoActors) {
	
	unordered_map<string, ActorNode*>::const_iterator actor_itr_1 = graph.all_actor.find(a.first);	
	unordered_map<string, ActorNode*>::const_iterator actor_itr_2 = graph.all_actor.find(a.second);
	if(actor_itr_1 == graph.all_actor.end() || actor_itr_2 == graph.all_actor.end()) {
	    cout << "One or both the actors not present in the map" << endl;
	    continue;	
	}
	if(actor_itr_1 == actor_itr_2) {
	    cout << "Both are the same actors" << endl;
	    continue;
	}
	ActorNode* actor_1 = actor_itr_1->second;
	ActorNode* actor_2 = actor_itr_2->second;
	if(weightedGraph) {
		Djikstra(actor_1, actor_2);
		print_path(actor_2, ofs);
	}
	else {
		BFS(actor_1, actor_2);
		print_path(actor_2, ofs);
	}
	graph.reset_vitals();
    }
    ofs.close();
    return 0;
}


void Djikstra(ActorNode* actor_1, ActorNode* actor_2)
{
    //Using priority queue with custom comparator(defined in ActorNode.hpp)
    priority_queue<ActorNode*, vector<ActorNode*>, CompareActorNode> p_queue; 
    actor_1->distance = 0;
    p_queue.push(actor_1);

    while(!(p_queue.empty())) {
	ActorNode* curr = p_queue.top();
	p_queue.pop();
	
	if(curr->actor_name == actor_2->actor_name) { // && curr->visited) {
	    break;
	}	
	
	if(!(curr->visited)) {
	   
	    curr->visited = true;
	    for(int i=0; i < curr->movie_list.size(); i++) {
		for(int j=0; j < curr->movie_list[i]->actor_list.size(); j++) {
		    if(curr->movie_list[i]->actor_list[j]->visited == false) {
		        ActorNode* adj_curr = curr->movie_list[i]->actor_list[j];
		        int adj_distance = curr->distance + curr->movie_list[i]->weight;
		        // Now there are various conditions to match
		        // 1. We only update the distance, and previous 
		        // if the new distance is lesser than the 
		        // previous distance or if those field are 
		        // not initialized.
		        // 2. The actor's movie_lists's actor list will have
		        // the actor himself, so the actor should not be
		        // added to the priority queue. 
		        if((adj_distance < adj_curr->distance || adj_curr->distance == -1) && (curr->actor_name != adj_curr->actor_name)) {
			    adj_curr->distance = adj_distance;
			    adj_curr->prev_actor = curr;
			    adj_curr->prev_movie = curr->movie_list[i];
 			    p_queue.push(adj_curr);
			}
			
		    }
		}
	    }
	     
	}

    }


}

void BFS(ActorNode* actor_1, ActorNode* actor_2)
{
    queue<ActorNode*> q;
    actor_1->distance = 0;
    q.push(actor_1);
    while(!(q.empty())) {
	
	ActorNode* curr = q.front();
	q.pop();
	if(curr->actor_name == actor_2->actor_name) {
	    break;
	}

	if(curr->distance == -1 || curr == actor_1)
	{
	    for(int i=0; i < curr->movie_list.size(); i++) {
			
		for(int j=0; j < curr->movie_list[i]->actor_list.size(); j++) {
			
		    ActorNode* adj_curr = curr->movie_list[i]->actor_list[j];
		    int adj_distance = curr->distance + 1;
    		    if((adj_curr->distance == -1) && (adj_curr->prev_actor == NULL) && (adj_curr->actor_name != curr->actor_name) ) {
			adj_curr->prev_actor = curr;
			adj_curr->prev_movie = curr->movie_list[i];
			q.push(adj_curr);
		    }
		}
	    }
	}
    }

}

vector< pair<string, string> > loadInputFile(const char* inFileName)
{
    ifstream infile(inFileName);
    bool have_header = false;
    vector< pair<string, string> > inputActors;
    while(infile)
    {
        string s;
        
        //get the next line
        if(!getline( infile, s)) break;
        
        if(! have_header)
        {
            //skip the header
            have_header = true;
            continue;
        }
        
        istringstream ss( s );
        vector<string> record;

        while(ss)
        {
            string next;
            //get the next string before hitting a tab character and put it in 'text'
            if(!getline( ss, next, '\t')) break;
            record.push_back( next );
        }
        
        if(record.size() != 2)
        {
            continue;
        }
        
        string actor1(record[0]);
        string actor2(record[1]);
        
        pair <string, string> pairActor (actor1, actor2);
        inputActors.push_back(pairActor);
    }

    if(!infile.eof())
    {
        cerr << "Failed to read " << inFileName << "!\n";
        return inputActors;
    }
    infile.close();

    return inputActors;;
}

void print_path(ActorNode* last_actor, ofstream& ofs) {
    ActorNode* ref = last_actor;
    string path = "(" + ref->actor_name + ")";
    while(ref->prev_actor != nullptr) {
        string temp = "(" + ref->prev_actor->actor_name + ")" + "--" + "[" + ref->prev_movie->movie_name + "#@" + to_string(ref->prev_movie->movie_year) + "]-->";
        path = temp + path; 
        ref  = ref->prev_actor; 
    }
    ofs << path << endl; 
}
