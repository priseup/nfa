#include "nfa.h"
#include <string>
#include <vector>
#include <stack>

enum 
{
    SPLIT = 257,
    END = 258
};

inline State* createState(int c)
{
    return new State{c, nullptr, nullptr};
}

State* post2nfa(const std::string &str)
{
    std::stack<Fragment> sf;

    for (auto c : str)    
    {
        switch (c)
        {
        case '?':
            {
                auto f =  sf.top();
                sf.pop();

                auto split = createState(SPLIT);

                split->next2 = f.start;

                auto ends = f.ends;
                ends.push_back(split);

                sf.push(Fragment{split, ends});
                break;
           }
        case '+':
            {
                auto f =  sf.top();

                for (auto end : f.ends)
                    end->next2 = f.start;

                break;
            }
        case '*':
            {
                auto f =  sf.top();
                sf.pop();

                auto split = createState(SPLIT);

                split->next2 = f.start;

                for (auto end : f.ends)
                    end->next2 = f.start;

                auto ends = f.ends;
                ends.push_back(split);

                sf.push(Fragment{split, ends});
                break;
           }
        case '|':
            {
                auto second =  sf.top();
                sf.pop();
                auto first = sf.top();
                sf.pop();

                auto split = createState(SPLIT);

                split->next1 = first.start;
                split->next2 = second.start;

                auto ends = first.ends;
                ends.insert(ends.end(), second.ends.begin(), second.ends.end());

                sf.push(Fragment{split, ends});

                break;
            }
        case '~':
            {
                auto second =  sf.top();
                sf.pop();
                auto first = sf.top();
                sf.pop();

                for (auto end : first.ends)
                    end->next1 = second.start;

                sf.push(Fragment{first.start, second.ends});
                break;
            }
        default:
            {
                auto s = createState(c);

                sf.push(Fragment{s, std::vector<State*>{s}});
                break;
            }
        }
    }

    // the END flag
    auto s = createState(END);
    for (auto end : sf.top().ends)
        end->next1 = s;

    return sf.top().start;
}

void addState(State *s, std::vector<State*> &next)
{
    if (s == nullptr)
        return;

	if (s->state != SPLIT)
	{
		next.push_back(s);
		return ;
	}
    else
    {
		addState(s->next1, next);
		addState(s->next2, next);
    }
}

bool is_match(const std::vector<State*> states)
{
    for (auto state : states)
    {
        if (state->state == END)
            return true;
    }

    return false;
}

void step(std::vector<State*> &current, int c, std::vector<State*> &next)
{
    for (auto state : current)
    {
        if (state->state == c)
        {
            addState(state->next1, next);
            addState(state->next2, next);
		}
    }
    current = next;
    next.clear();
}

bool match(const std::string &str, State *start)
{
    std::vector<State*> current_states, next_states;

    addState(start, current_states);
    for (auto c : str)
    {
        step(current_states, c, next_states);
    }

    return is_match(current_states);
}


