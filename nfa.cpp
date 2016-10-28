#include "nfa.h"
#include <string>
#include <vector>
#include <stack>
#include <utility>
#include <algorithm>

unsigned state_id = 0;

inline State *createState(unsigned i, Edge e1, Edge e2)
{
    State *s = new State{i, e1, e2};

    return s;
}

Fragment constructFragment(char c)
{
    Edge e{-2, nullptr};

    State *start = createState(state_id++, e, e);
    State *end = createState(state_id++, e, e);

    start->e1 = {c, end};

    return {start, end};
}

State *post2nfa(const std::string &s)
{
    std::stack<Fragment> sf;
    State *s1 = nullptr, *s2 = nullptr;

    for (auto c : s)    
    {
        switch (c)
        {
        case '?':
            {
                auto f = sf.top();
                sf.pop();

                Edge e1{-1, f.start};
                Edge e2{-2, nullptr};

                s1 = createState(state_id++, e1, e2);
                s2 = createState(state_id++, e2, e2);

                s1->e2 = {-1, s2};
                f.end->e2 = {-1, s2};

                sf.push(Fragment{s1, s2});
                break;
            }
        case '+':
            {
                auto f = sf.top();
                sf.pop();

                Edge e1{-1, f.start};
                Edge e2{-2, nullptr};

                s1 = createState(state_id++, e1, e2);
                s2 = createState(state_id++, e2, e2);

                f.end->e1 = {-1, f.start};
                f.end->e2 = {-1, s2};

                sf.push(Fragment{s1, s2});
                break;
            }
        case '*':
            {
                auto f = sf.top();
                sf.pop();

                Edge e1{-1, f.start};
                Edge e2{-2, nullptr};

                s1 = createState(state_id++, e1, e2);
                s2 = createState(state_id++, e2, e2);

                s1->e2 = {-1, s2};

                f.end->e2 = {-1, f.start};
                f.end->e1 = {-1, s2};

                sf.push(Fragment{s1, s2});
                break;
            }
        case '|':
            {
                auto second =  sf.top();
                sf.pop();
                auto first = sf.top();
                sf.pop();

                Edge e1{-1, first.start};
                Edge e2{-1, second.start};
                s1 = createState(state_id++, e1, e2);

                e1 = e2 = {-2, nullptr};
                s2 = createState(state_id++, e1, e2);

                first.end->e1 = {-1, s2};
                second.end->e1 = {-1, s2};

                sf.push(Fragment{s1, s2});
                break;
            }
        case '~':
            {
                auto second =  sf.top();
                sf.pop();
                auto first = sf.top();
                sf.pop();

                first.end->e1 = {-1, second.start};

                sf.push(Fragment{first.start, second.end});
                break;
            }
        default:
            {
                sf.push(std::move(constructFragment(c)));
                break;
            }
        }
    }

    return sf.top().start;
}

std::vector<Edge> addState(State *s)
{
    std::vector<Edge> next;
	if (s->e1.match_content != -1
		&& s->e1.match_content != -2)
	{
		next.push_back(s->e1);
		return next;
	}
    while (s->e1.match_content == -1
        && s->e2.match_content == -2)
    {
        s = s->e1.to_state;
    }

    if (s->e1.match_content == -1
        && s->e1.match_content == -1)
    {
		auto tmp = addState(s->e1.to_state);
        next.insert(next.end(), tmp.begin(), tmp.end());
        
		tmp = addState(s->e2.to_state);
		next.insert(next.end(), tmp.begin(), tmp.end());
	}
    
    return next;
}

bool is_match(const std::vector<Edge> &es)
{
    auto end_id = --state_id;
    
	for (auto edge : es)
	{
		auto state = edge.to_state;
		while (state && state->sequence != end_id)
		{
			state = state->e1.to_state;
		}
		if (state && state->sequence == end_id)
			return true;
	}

    return false;
}

void step(std::vector<Edge> &current, int c, std::vector<Edge> &next)
{
    for (auto edge : current)
    {
        if (edge.match_content == c)
        {
            auto edges = addState(edge.to_state);
			next.insert(next.end(), edges.begin(), edges.end());
		}

    }
}

bool match(const std::string &str, State *start)
{
    auto current_edges = addState(start);
    decltype(current_edges) next_edges;

    for (auto c : str)
    {
        step(current_edges, c, next_edges);
		if (next_edges.empty())
		{
			for (auto it = current_edges.begin();
				it != current_edges.end(); ++it)
				if (it->match_content != c)
				{
					it = current_edges.erase(it);
					if (it == current_edges.end())
						break;
				}
		}
		else
		{
			current_edges = next_edges;
		}
        next_edges.clear();
    }

    return is_match(current_edges);
}
