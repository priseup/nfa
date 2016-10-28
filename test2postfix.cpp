#include "str2postfix.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char **argv)
{
    std::string input;


    unsigned i = 1;
    while (i < argc)
    {
        input = argv[i++];
        std::cout << input << std::endl;
        std::cout << "postfix string expression are: " << std::endl;
        std::cout << infix2post(input) << std::endl;
        std::cout << std::endl;
    }
	
//    input = "(a|b)*|abb";
//    input = "(a|b)*abb";
//    input = "(a*|b|c)";
//    std::cout << input << std::endl;
//    std::cout << "postfix string expression are: " << std::endl;
//    std::cout << infix2post(input) << std::endl;
//    std::cout << std::endl;

    std::system("pause");

    return 0;
}
