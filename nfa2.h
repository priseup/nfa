#ifndef NFA2_H
#define NFA2_H

#include <string>

struct State;
struct Edge
{
    int match_content;
    State* to_state;
};

struct State
{
    int sequence;
    Edge e1;
    Edge e2;
};

struct Fragment
{
    State* start;
    State* end;
};

State* post2nfa(const std::string &s);
bool match(const std::string &str, State* start);


#endif // NFA2_H
