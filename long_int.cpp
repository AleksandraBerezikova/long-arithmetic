#include <stdexcept>
#include <algorithm>

#include "long_int.hpp"

LongInt::LongInt()
    : digits(0), negative(false)
{}

LongInt::LongInt(LongInt const &number)
{
    if (this != &number)
    {
        this->negative = number.negative;
        this->digits = number.digits;
    }
}

LongInt::LongInt(std::string number)
{
    if (number == "")
    {
        throw std::invalid_argument("number cannot be an empty string!");
    }
    
    size_t start = 0;
    negative = false;
    if (number[0] == '-' || number[0] == '+')
    {
        negative = number[0] == '-';
        start = 1;
        if (number.size() == 1)
        {
            throw std::invalid_argument("number is an inappropriate string!");
        }

        if (number[1] == '0')
        {
            negative = false;
        }
    }
    if (number[start] == '0' && number.length() != start + 1)
    {
        throw std::invalid_argument("number is an inappropriate string!");
    }

    for (size_t i = start; i < number.length(); ++i)
    {
        if (number[i] >= '0' && number[i] <= '9')
        {
            digits.push_back(number[i] - '0');
        }
        else 
        {
            throw std::invalid_argument("number is an inappropriate string!");
        }
    }
}

LongInt::LongInt(long long number)
{
    if (number < 0)
    {
        negative = true;
        number *= -1;
    }
    else
    {
        negative = false;
    }
    int next_digit;
    if (number == 0)
    {
        digits.push_back(number);
    }
    else
    {
        while (number != 0)
        {
            next_digit = number % 10;
            number /= 10;
            digits.push_back(next_digit);
        }
        reverse(digits.begin(), digits.end());
    }
}

LongInt &LongInt::operator=(LongInt const &other)
{
    if (this != &other)
    {
        this->negative = other.negative;
        this->digits = other.digits;
    }
    return *this;
}

bool LongInt::operator==(LongInt const &second_number) const
{
    if (this->digits.size() == second_number.digits.size())
    {
        if (this->digits.size() == 1 && this->digits[0] == 0 && second_number.digits[0] == 0)
        {
            return true;
        }
        if (this->negative != second_number.negative)
        {
            return false;
        }
        for (size_t i = 0; i < this->digits.size(); ++i)
        {
            if (this->digits[i] != second_number.digits[i])
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

std::string LongInt::to_string() const
{
    std::string number;
    
    if (negative == true)
    {
        number.push_back('-');
    }
    for (size_t i = 0; i < digits.size(); ++i)
    {
        number += (digits[i] + '0');
    }

    return number;
}

bool LongInt::is_negative()
{
    return negative;
}

LongInt &LongInt::operator+=(LongInt const &second_number)
{
    if (!this->negative && second_number.negative)//a+-b
    {
        LongInt temp_positive_second_number(second_number);
        temp_positive_second_number.negative = false;
        *this = *this - temp_positive_second_number;
    }
    else if (this->negative && !second_number.negative)//-a+b
    {
        LongInt temp_positive_first_number(*this);
        temp_positive_first_number.negative = false;
        *this = second_number - temp_positive_first_number;
    }
    else
    {
        auto sum_digits = add_vectors(digits, second_number.digits);
        this->digits = sum_digits;
        return *this;
    }
   return *this;
}

LongInt &LongInt::operator-=(LongInt const &second_number)
{
    if (this->negative != second_number.negative)//a- -b
    {
        this->digits = add_vectors(this->digits, second_number.digits);
    }
    else 
    {
        if ((*this < second_number && !negative) || ((*this > second_number) && negative))
        {
            
            this->digits = sub_vectors(second_number.digits, this->digits);
            negative ^= true;
        }
        else
        {
            this->digits = sub_vectors(this->digits, second_number.digits);
            if (this->digits[0] == 0) 
            {
                this->negative = false;
            }
        }
    }
    
    return *this;
}

LongInt &LongInt::operator*=(LongInt const &second_number)
{
    this->digits = multip_vectors(this->digits, second_number.digits);
    if (this->negative != second_number.negative)
    {
        this->negative = true;
    }
    return *this;
}

bool LongInt::operator<(LongInt const &second_number) const
{
    if (this->negative && !second_number.negative)
    {
        return true;
    }
    else if (!this->negative && second_number.negative)
    {
        return false;
    }

    if(this->digits.size() < second_number.digits.size())
    {
        return true;
    }
    else if(this->digits.size() > second_number.digits.size())
    {
        return false;
    }
    else if(this->digits.size() == second_number.digits.size())
    {
        for (size_t i = 0; i < this->digits.size(); ++i)
        {
            if (this->digits[i] < second_number.digits[i])
            {
                if (!this->negative)
                {
                    return true;
                }
                return false;
                
            }
            else if (this->digits[i] > second_number.digits[i])
            {
                if (!this->negative)
                {
                    return false;
                }
                return true;           
            }
        }
    }
    return false;
}

bool LongInt::operator>(LongInt const &second_number) const
{
    return !(*this < second_number || *this == second_number);
}

LongInt operator+(LongInt first_number, LongInt const &second_number)
{
    return first_number += second_number;
}

LongInt operator-(LongInt first_number, LongInt const &second_number)
{
    return first_number -= second_number;
}

LongInt operator*(LongInt first_number, LongInt const &second_number)
{
    return first_number *= second_number;
}

std::vector<char> add_vectors(std::vector<char> const &digits1, std::vector<char> const &digits2)
{
    int length_first_number = digits1.size() - 1;
    int length_second_number = digits2.size() - 1;
    std::vector<char> sum_result;
    int sum;
    int carry = 0;
    while (length_first_number >= 0 && length_second_number >= 0)
    {
        sum = carry + digits1[length_first_number] + digits2[length_second_number];
        if (sum > 9)
        {
            carry = 1;
            sum = sum - 10;
        }
        else
        {
            carry = 0;
        }
        sum_result.push_back(sum);
        --length_first_number;
        --length_second_number;
    }

    auto* remaining_digits = &digits2;
    int remaining_length = length_second_number;
    if (length_first_number >= 0 && length_second_number < 0)
    {
        remaining_digits = &digits1;
        remaining_length = length_first_number;
    }

    for (int i = remaining_length; i >= 0; --i)
    {
        sum = carry + (*remaining_digits)[i];
        if (sum > 9)
        {
            carry = 1;
            sum = sum - 10;
        }
        else
        {
            carry = 0;
        }
        sum_result.push_back(sum);
    }

    reverse(sum_result.begin(), sum_result.end());

    return sum_result;
}



std::vector<char> sub_vectors(std::vector<char> const &digits1, std::vector<char> const &digits2)
{
    int carry = 0;
    int sub;
    int length_first_number = digits1.size() - 1;
    int length_second_number = digits2.size() - 1;
    std::vector<char> sub_result;

    while(length_first_number >= 0 && length_second_number >= 0)
    {
        sub = int(digits1[length_first_number]) - digits2[length_second_number] - carry;
        if (sub < 0)
        {
            carry = 1;
            sub = sub + 10;
        }
        else
        {
            carry = 0;
        }
        sub_result.push_back(sub);
        --length_first_number;
        --length_second_number;
    }
    if (length_first_number >= 0 && length_second_number < 0)
    {
        for (int i = length_first_number; i >= 0; --i)
        {
            sub = digits1[i] - carry;
            if (sub < 0)
            {
                carry = 1;
                sub = sub + 10;
            }
            else
            {
                carry = 0;
            }
            sub_result.push_back(sub);
        }
    }
    reverse(sub_result.begin(), sub_result.end());

    int leading_zeros = 0;
    for (size_t i = 0; i < sub_result.size() - 1; ++i)
    {
        if (sub_result[i] == 0)
        {
            ++leading_zeros;
        } 
        else
        {
            break;
        }
        
    }

    sub_result = std::vector<char>(sub_result.begin() + leading_zeros, sub_result.end());
    return sub_result;

}

std::vector<char> multip_vectors(std::vector<char> const &digits1, std::vector<char> const &digits2)
{
    int carry = 0;
    int multip = 0;
    std::vector<char> multip_result(digits1.size() + digits2.size() - 1, 0);
    for (int i = digits1.size() - 1; i >= 0; --i)
    {
        carry = 0;
        for (int j = digits2.size() - 1; j >= 0 || carry; --j)
        {
            multip = carry + digits1[i] * (j >= 0 ? digits2[j] : 0) + multip_result[i + j];
            carry = multip / 10;
            multip_result[i + j] = multip % 10;
        }
    }

    int leading_zeros = 0;
    for (size_t i = 0; i < multip_result.size() - 1; ++i)
    {
        if (multip_result[i] == 0)
        {
            ++leading_zeros;
        } 
        else
        {
            break;
        }
    }
    multip_result = std::vector<char>(multip_result.begin() + leading_zeros, multip_result.end());
    return multip_result;
}
