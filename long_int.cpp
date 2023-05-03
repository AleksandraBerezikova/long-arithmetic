#include <stdexcept>
#include <algorithm>


#include "long_int.hpp"

LongInt::LongInt(LongInt const &number)
{
    this->negative = number.negative;
    for (size_t i  = 0; i < number.digits.size(); ++i)
    {
        this->digits.push_back(number.digits[i]);
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
    
    if (negative == true && digits[0] != 0)
    {
        number.push_back('-');
    }
    for (size_t i = 0; i < digits.size(); ++i)
    {
        number.push_back(digits[i] + '0');
    }

    return number;
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
    else if (this->negative && second_number.negative) //-a+-b
    {
        LongInt temp_positive_first_number(*this);
        LongInt temp_positive_second_number(second_number);
        temp_positive_first_number.negative = false;
        temp_positive_second_number.negative = false;
        *this = temp_positive_second_number + temp_positive_first_number;
        this->negative = true;
    }
    else
    {
        int length_first_number = this->digits.size() - 1;
        int length_second_number = second_number.digits.size() - 1;
        std::vector<char> sum_result;
        int sum;
        int carry = 0;
        while (length_first_number >= 0 && length_second_number >= 0)
        {
            sum = carry + this->digits[length_first_number] + second_number.digits[length_second_number];
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
        if (length_first_number < 0 && length_second_number >= 0)
        {
            for (int i = length_second_number; i >= 0; --i)
            {
                sum = carry + second_number.digits[i];
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
        }
        else if (length_first_number >= 0 && length_second_number < 0)
        {
            for (int i = length_first_number; i >= 0; --i)
            {
                sum = carry + this->digits[i];
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
        }
        reverse(sum_result.begin(), sum_result.end());
        this->digits = sum_result;
        return *this;
    }
   return *this;
}

LongInt &LongInt::operator-=(LongInt const &second_number)
{
    if (!this->negative && second_number.negative)//a- -b
    {
        LongInt temp_positive_second_number(second_number);
        temp_positive_second_number.negative = false;
        *this = *this + temp_positive_second_number;
        if (*this < second_number)
        {
            this->negative = true;
        }
        else
        {
            this->negative = false;
        }
    }
    else if (this->negative && !second_number.negative)//-a-+b
    {
        LongInt temp_positive_first_number(*this);
        temp_positive_first_number.negative = false;
        *this = second_number + temp_positive_first_number;
        this->negative = true;

    }
    else if (this->negative && second_number.negative)//-a- -b
    {
        LongInt temp_positive_second_number(second_number);
        LongInt temp_positive_first_number(*this);
        temp_positive_second_number.negative = false;
        temp_positive_first_number.negative = false;
        *this = temp_positive_second_number - temp_positive_first_number;
        if (*this < second_number)
        {
            this->negative = true;
        }
        else
        {
            this->negative = false;
        }
    }
    else 
    {
        if (*this < second_number)
        {
            *this = second_number - *this;
            this->negative = true;
        }
        else
        {
            int carry = 0;
            int sub;
            int length_first_number = this->digits.size() - 1;
            int length_second_number = second_number.digits.size() - 1;
            std::vector<char> sub_result;

            while(length_first_number >= 0 && length_second_number >= 0)
            {
                sub = this->digits[length_first_number] - second_number.digits[length_second_number] - carry;
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
                    sub = this->digits[i] - carry;
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
        this->digits = sub_result;

        std::vector<char> temp_result;
        bool extra_zero = true;
        for (size_t i = 0; i < this->digits.size(); ++i)
        {
            while (this->digits[i] == 0 && extra_zero && i < this->digits.size())
            {
                ++i;
            }
            extra_zero = false;
            temp_result.push_back(this->digits[i]);
            
        }
        this->digits = temp_result;
        return *this;
        }
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

LongInt operator+(LongInt first_number, LongInt const &second_number)
{
    return first_number += second_number;
}

LongInt operator-(LongInt first_number, LongInt const &second_number)
{
    return first_number -= second_number;
}
