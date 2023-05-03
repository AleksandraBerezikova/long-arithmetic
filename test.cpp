#include <iostream>
#include <string>
#include <cassert>

#include "long_int.hpp"

using namespace std;

void assert_string_constructor_throws(string number, string error_msg) 
{
    try
    {
        LongInt a(number);
    }
    catch(const std::invalid_argument& e)
    {
        if (std::string(e.what()) != error_msg) {
            cout << "Wrong exception message, got: " << e.what() << endl;
            assert(0);
        }
        return;
    }

    cout << "Constructor from string didn't throw expected exception on input: " << number << endl;
    assert(0);
}

void test_string_constructor_negative()
{
    assert_string_constructor_throws("", "number cannot be an empty string!");
    assert_string_constructor_throws("+", "number is an inappropriate string!");
    assert_string_constructor_throws("-", "number is an inappropriate string!");
    assert_string_constructor_throws("+015", "number is an inappropriate string!");
    assert_string_constructor_throws("+98123211ujas5", "number is an inappropriate string!");

    cout << "test_string_constructor_negative passed!" << endl;
}


void test_equals()
{
    assert(LongInt("15") == LongInt(15));
    assert(LongInt("0") == LongInt(0));
    assert(LongInt("-0") == LongInt("+0"));
    assert(LongInt(0) == LongInt("-0"));
    assert(LongInt(-9123123123) == LongInt("-9123123123"));

    assert(!(LongInt(-22) == LongInt("22")));
    assert(!(LongInt(-0) == LongInt(1)));


    cout << "test_equals passed!" << endl;
} 
void test_to_string()
{
    assert(LongInt("15").to_string() == "15");
    assert(LongInt("-0").to_string() == "0");
    assert(LongInt("+0").to_string() == "0");
    assert(LongInt("-12").to_string() == "-12");
    string s = "578495793179537594376317509347659769719305899576";
    assert(LongInt(s).to_string() == s);

    cout << "test_to_string passed!" << endl;
} 
void test_less()
{
    assert(LongInt("15") < LongInt(16) == true);
    assert(LongInt(-987) < LongInt(16) == true);
    assert(LongInt(39488) < LongInt(0) == false);


    cout << "test_less passed!" << endl;
} 
void test_sum()
{
    int x = 0;
    assert(LongInt("15") + LongInt(15) == LongInt(30));
    assert(LongInt("15") + LongInt(-15) == LongInt(0));
    assert(LongInt("-98") + LongInt(76) == LongInt(-22));
    assert(LongInt("-8") + LongInt(-15) == LongInt(-23));
    assert(LongInt("-0") + LongInt(-0) == LongInt(0));
    assert(LongInt("+0") + LongInt(-15) == LongInt(-15));
    assert(LongInt("0") + LongInt(x) == LongInt(0));
    assert(LongInt("-0") + LongInt(-0) == LongInt(0));
    assert(LongInt("34783537829847836534787918348365763568710") + LongInt(67) == LongInt("34783537829847836534787918348365763568777"));
    cout << "test_sum passed!" << endl;
} 

void test_sub()
{
    assert(LongInt("-15") - LongInt(15) == LongInt(-30));
    assert(LongInt("-0") - LongInt(0) == LongInt(0));
    assert(LongInt("+0") - LongInt(15) == LongInt(-15));
    assert(LongInt("14") - LongInt("-15") == LongInt(29));
    assert(LongInt("34") - LongInt("15") == LongInt("19"));
    assert(LongInt("-14") - LongInt("-15") == LongInt(1));
    assert(LongInt("-0") - LongInt("+0") == LongInt(0));
    assert(LongInt("-1") - LongInt("-1") == LongInt(0));
    assert(LongInt("1000000000000000000000000000000") - LongInt("1000000000000000000000000000000") == LongInt(0));
    assert(LongInt("34783537829847836534787918348365763568710") - LongInt(67) == LongInt("34783537829847836534787918348365763568643"));
    cout << "test_sub passed!" << endl;
} 

int main() 
{
    test_string_constructor_negative();
    test_equals();
    test_to_string();
    test_less();
    test_sum();
    test_sub();

    return 0;
}