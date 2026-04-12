#include "../../source/util.h"
#include <iostream>

int main()
{
    std::string random_str = aries_util::RANDOM::generateRandomString(6, aries_util::StrType::NUMBER);
    std::cout << random_str << std::endl;
    return 0;
}