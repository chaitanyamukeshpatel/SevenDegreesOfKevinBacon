#ifndef MOVIENODE_HPP
#define MOVIENODE_HPP

#include <iostream>
#include "ActorNode.hpp"
#include <string>
#include <vector>
#include <unordered_map>
// Maybe include some data structures here

using namespace std;
class ActorNode;
class MovieNode {
public:
    //unordered_map<string, ActorNode*> actor_list;
    vector<ActorNode*> actor_list;
    string movie_name;
    int weight;
    int movie_year;
    MovieNode(string name, int year) 
    {
	weight = 1 + (2018-year);
	movie_name = name;
	movie_year = year;
			
    }
};


#endif // ACTORNODE_HPP
