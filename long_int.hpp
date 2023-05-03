#ifndef LONG_INT
#define LONG_INT

#include <vector>
#include <string>

class LongInt
{
private:
    std::vector<char> digits;
    bool negative;

public:
    LongInt(LongInt const &number);
    explicit LongInt(std::string number);
    explicit LongInt(long long number);
    LongInt& operator=(LongInt const& number);
    bool operator==(LongInt const& second_number) const;
    std::string to_string() const;
    LongInt& operator+=(LongInt const &second_number);
    LongInt& operator-=(LongInt const &second_number);
    bool operator<(LongInt const& second_number) const;
};

LongInt operator+(LongInt first_number, LongInt const& second_number);
LongInt operator-(LongInt first_number, LongInt const& second_number);

#endif