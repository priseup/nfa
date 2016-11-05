#ifndef NFA_H
#define NFA_H

#include <vector>
#include <string>

struct State
{
    int state;
    State *next1;
    State *next2;
};

struct Fragment
{
    State *start;
    std::vector<State*> ends;
};

State* post2nfa(const std::string &s);
bool match(const std::string &str, State* start);

#endif		// NFA_H
