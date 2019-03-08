/*
 * ActorGraph.cpp
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */
 
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "ActorGraph.hpp"
using namespace std;

ActorGraph::ActorGraph(void) {}

bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {
    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;
     
    int idx = 0;
    // keep reading lines until the end of file is reached
    while (infile) {
        string s;
    
        // get the next line
        if (!getline( infile, s )) break;

        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss( s );
        vector <string> record;
        while (ss) {
            string next;
      
            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }
    
        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);
    
        // we have an actor/movie relationship, now what?
        // There are total 4 different cases, one where 
        // the actor already is in all_actors vector,
        // but the movie is not in all_movies vector.
        // One where movie and actor are both in their
        // respective list. And then there are cases 
        // where actor is not in the list and the movie 
        // is, and where neither actor nor the movie are
        // in their respective list.
        string unique_movie = movie_title + to_string(movie_year);
	bool actor_found = false;
	bool movie_found = false;
	unordered_map<string, ActorNode*>::const_iterator actor_itr = all_actor.find(actor_name);
	unordered_map<string, MovieNode*>::const_iterator movie_itr = all_movie.find(unique_movie);
	if(actor_itr != all_actor.end()) {
		actor_found = true;
	}
	
	if(movie_itr != all_movie.end()) {
		movie_found = true;
	}
	
	if(actor_found && movie_found) {
		actor_itr->second->movie_list.push_back(movie_itr->second);
		movie_itr->second->actor_list.push_back(actor_itr->second);
	}
	else if(actor_found && (!movie_found)) {
		MovieNode* movie = new MovieNode(movie_title, movie_year);
		all_movie.insert(make_pair(unique_movie, movie));
		actor_itr->second->movie_list.push_back(movie);
		movie->actor_list.push_back(actor_itr->second); 
	}
	else if((!actor_found) && movie_found) {	
		ActorNode* actor = new ActorNode(actor_name);
		actor->index = idx;
		idx++;
		all_actor.insert(make_pair(actor_name, actor));
		index_actor.insert(make_pair(actor->index, actor_name));
		movie_itr->second->actor_list.push_back(actor);
		actor->movie_list.push_back(movie_itr->second);
	}
	else if((!actor_found) && (!movie_found)) {	
		MovieNode* movie = new MovieNode(movie_title, movie_year);
		all_movie.insert(make_pair(unique_movie, movie));
		ActorNode* actor = new ActorNode(actor_name);
		actor->index = idx;
		idx++;
		all_actor.insert(make_pair(actor_name, actor));
		index_actor.insert(make_pair(actor->index, actor_name));
		movie->actor_list.push_back(actor);
		actor->movie_list.push_back(movie);	
	}
	
    }//end of while
    adj_matrix.resize(all_actor.size(), vector<int>(all_actor.size(),0));
    for(auto a: all_actor) {
	for(auto m: a.second->movie_list) {
	    for(auto a2: m->actor_list) {
		if(a2->actor_name == a.second->actor_name) {
		    //continue;
		}
		else if(adj_matrix[a.second->index][a2->index] != 1) {
		    adj_matrix[a.second->index][a2->index] = 1;
		    a.second->degree++;
		}
	    }

	}

    }
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();

    return true;
}
void ActorGraph::reset_vitals() {
    for(auto actor_itr = all_actor.begin(); actor_itr != all_actor.end(); ++(actor_itr)) {	
	actor_itr->second->distance = -1;
	actor_itr->second->visited = false;
	actor_itr->second->prev_movie = NULL;
	actor_itr->second->prev_actor = NULL;
    }

}

ActorGraph::~ActorGraph() {
    for(auto actor_itr = all_actor.begin(); actor_itr != all_actor.end(); ++(actor_itr)) {	
	delete actor_itr->second;
    }
    for(auto movie_itr = all_movie.begin(); movie_itr != all_movie.end(); ++(movie_itr)) {	
	delete movie_itr->second;
    }
}
