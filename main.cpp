#include <iostream>

#include "long_int.hpp"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " first_number second_number\n";
        return 1;
    }

    std::string first_number, second_number;
    first_number = argv[1];
    second_number = argv[2];

    LongInt num_1(first_number);
    LongInt num_2(second_number);

    std::cout << first_number << " + " << second_number << " = " << (num_1 + num_2).to_string() << '\n';
    std::cout << first_number << " - " << second_number << " = " << (num_1 - num_2).to_string() << '\n';

    return 0;
}