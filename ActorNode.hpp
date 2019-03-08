#ifndef ACTORNODE_HPP
#define ACTORNODE_HPP

#include <iostream>
#include <vector>
#include "MovieNode.hpp"
#include <unordered_map>
// Maybe include some data structures here

using namespace std;
class MovieNode;
class ActorNode {
public:
    vector<MovieNode*> movie_list;
    //unordered_map<string, MovieNode*> movie_map;
    ActorNode(string name)
    {
	actor_name = name;
	distance = -1;
	prev_actor = NULL;
	prev_movie = NULL;
	visited = false;
	index = -1;
        degree = 0;			
    }
    string actor_name;
    int distance;
    ActorNode* prev_actor;
    MovieNode* prev_movie;
    bool visited;
    int index;
    int degree;
};

class CompareActorNode {
public:
	bool operator()(ActorNode*& left, ActorNode*& right) const {
		if((*left).distance != (*right).distance) {
			return (*left).distance > (*right).distance;
		}
		else {
			//Sort alphabetically
			return (*left).actor_name > (*right).actor_name;
		}
	}

};
#endif // ACTORNODE_HPP
