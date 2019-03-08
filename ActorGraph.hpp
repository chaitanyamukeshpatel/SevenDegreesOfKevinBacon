/*
 * ActorGraph.hpp
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <iostream>
#include "ActorNode.hpp"
#include "MovieNode.hpp"
#include <unordered_map>
// Maybe include some data structures here

using namespace std;

class ActorGraph {
  
    // Maybe add class data structure(s) here
    //vector<ActorNode*> all_actor;
    //vector<MovieNode*> all_movie;
    //unordered_map<string, ActorNode*> all_actor;
    //unordered_map<string, MovieNode*> all_movie;
public:
    ActorGraph(void);
    ~ActorGraph();
    // Maybe add some more methods here
  
    /** You can modify this method definition as you wish
     *
     * Load the graph from a tab-delimited file of actor->movie relationships.
     *
     * in_filename - input filename
     * use_weighted_edges - if true, compute edge weights as 1 + (2018 - movie_year), otherwise all edge weights will be 1
     *
     * return true if file was loaded sucessfully, false otherwise
     */
    bool loadFromFile(const char* in_filename, bool use_weighted_edges);
    void reset_vitals();
    unordered_map<string, ActorNode*> all_actor;
    unordered_map<string, MovieNode*> all_movie;
    unordered_map<int, string> index_actor;
    vector<vector<int>> adj_matrix;

};


#endif // ACTORGRAPH_HPP
