#include "nfa2.h"
#include <string>
#include <vector>
#include <stack>
#include <utility>
#include <algorithm>


enum SSS
{
    SPLIT = -2,
    END = -1
};

int state_id = 0;

inline State* createState(int i, Edge e1, Edge e2)
{
    return new State{i, e1, e2};
}

Fragment constructFragment(char c)
{
    Edge e{-2, nullptr};

    auto start = createState(state_id++, e, e);
    auto end = createState(state_id++, e, e);

    start->e1 = {c, end};

    return {start, end};
}

State* post2nfa(const std::string &str)
{
    std::stack<Fragment> sf;
    State *s1 = nullptr, *s2 = nullptr;

    for (auto c : str)    
    {
        switch (c)
        {
        case '?':
            {
                auto f = sf.top();
                sf.pop();

                Edge e1{-1, f.start};
                Edge e2{-2, nullptr};

                s1 = createState(SPLIT, e1, e2);
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

                s1 = createState(SPLIT, e1, e2);
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
                s1 = createState(SPLIT, e1, e2);

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
                sf.push(constructFragment(c));
                break;
            }
        }
    }
    Edge e{-2, nullptr};
    auto s = createState(END, e, e);

	sf.top().end->e1 = { -1, s };

    return sf.top().start;
}

void addState(State* s, std::vector<State*> &nexts)
{
    if (s == nullptr)
        return ;

    if (s->sequence == SPLIT)
    {
        addState(s->e1.to_state, nexts);
        addState(s->e2.to_state, nexts);
    }
    else
    {
        nexts.push_back(s);
		if (s->e1.match_content >= 0)
			return;
    
		auto next1 = s->e1.to_state;
        addState(next1, nexts);
        auto next2 = s->e2.to_state;
        addState(next2, nexts);
    }
}

bool is_match(const std::vector<State*> &states)
{
	for (auto s : states)
	{
		if (s->sequence == END)
			return true;
	}

    return false;
}

void step(std::vector<State*> &currents, char c, std::vector<State*> &nexts)
{
    for (auto current : currents)
    {
        if (current->e1.match_content == c)
        {
			if (current->e1.to_state->e1.match_content == -1
				&& current->e1.to_state->e2.match_content == -2)
				 addState(current->e1.to_state->e1.to_state, nexts);
			else
				addState(current->e1.to_state, nexts);
		}
        if (current->e2.match_content == c)
        {
			if (current->e2.to_state->e1.match_content == -1
				&& current->e2.to_state->e2.match_content == -2)
				addState(current->e2.to_state->e1.to_state, nexts);
			else
				addState(current->e2.to_state, nexts);
		}
    }
}

bool match(const std::string &str, State* start)
{
    std::vector<State*> current_states, next_states;
    addState(start, current_states);

    for (auto c : str)
    {
        step(current_states, c, next_states);
        current_states = next_states;
        next_states.clear();
    }

    return is_match(current_states);
}
