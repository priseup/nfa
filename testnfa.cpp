#include <string>
#include "nfa.h"
#include "str2postfix.h"
#include <iostream>

int main()
{
	std::string big_string{ "aab" };
	std::string infix_re{ "(a|b)*b" };
	std::string post_re = infix2post(infix_re);
	State *start = post2nfa(post_re);

	bool match_result = match(big_string, start);
	std::cout << "infix: " << infix_re << std::endl;
    std::cout << "postfix: " << post_re << std::endl;
    if (match_result)
        std::cout << infix_re << "\t match \t" << big_string << std::endl;
    else
        std::cout << infix_re << "\t doesn't match \t" << big_string << std::endl;

	system("pause");
	return 0;
}
