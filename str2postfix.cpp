#include "str2postfix.h"
#include <stack>
#include <vector>
#include <algorithm>
#include <cctype>

namespace
{
    void moveOperator(std::stack<char> &operators, std::string &output,
                        const std::string &high_priorities, char c)
    {
        char top;
        while (!operators.empty())
        {
            top = operators.top();
            if (c == ')' && top != '(')
            {
                output.push_back(top);
                operators.pop();
            }
            else if (high_priorities.find(top) != std::string::npos)
            {
                output.push_back(top);
                operators.pop();
            }
            else
            {
                break;
            }
        }

        if (c != ')')
            operators.push(c);
        else if (c == ')')
            operators.pop();   // pop '('
    }

    std::string addConcatenate(const std::string &input)
    {
        if (input.size() < 2)
            return input;

        std::string output{input};

        size_t i = 0, j = i+1;
        while(i < output.size() - 1)
        {
            if (output[i]!='|' && output[i]!='(' && std::isalnum(output[j]))
            {
                output.insert(j, 1, '~');

                i += 2, j += 2;
            }
            else
            { 
                i++, j = i+1;
            }
        }

        return output;
    }
}


std::string infix2post(const std::string &infix)
{
    if (infix.size() < 2)
        return infix;

    std::string input = addConcatenate(infix);
    std::stack<char> operator_stack;
    std::string output;
    char top{0};


    std::vector<char> aster_befores{'*'};
    std::vector<char> and_befores{'~', '*'};
    std::vector<char> or_befores{'|', '~', '*'};
    for (auto c : input)
    {
        switch (c)
        {
            case '(':
                {
                operator_stack.push(c);
                break;
            }
        case ')':
            {
                moveOperator(operator_stack, output, "", ')');
                break;
            }
        case '*':
            {
                moveOperator(operator_stack, output, "*", '*');
                break;
            }
        case '~':
            {
                moveOperator(operator_stack, output, "~*", '~');
                break;
            }
        case '|':
            {
                moveOperator(operator_stack, output, "|~*", '|');
                break;
            }
        default:
            {
                output.push_back(c);
                break;
            }

        }
    }

    while (!operator_stack.empty())
    {
        top = operator_stack.top();

        output.push_back(top);
        operator_stack.pop();
    }

    return output;
}
