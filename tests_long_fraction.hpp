#include <iostream>
#include <assert.h>

#include "longfraction.h"

void test_constructor() {
    std::cout << "Test Constructor..." << std::endl;
    LongFraction t1;
    assert(t1.print() == "0.0");

    LongFraction t2(21234, 100);
    assert(t2.print() == "21234.0");

    LongFraction t3(-21234, 100);
    assert(t3.print() == "-21234.0");

    LongFraction t4(t2);
    assert(t2.print() == "21234.0");

    LongFraction t5 = t3;
    assert(t3.print() == "-21234.0");

    LongFraction t6("324664521234566786786694564678978");
    assert(t6.print() == "324664521234566786786694564678978.0");

    LongFraction t7("324664521234566786786694564678978.434656576768342553566");
    std::cout << t7.print() << std::endl;
    assert(t7.print() == "324664521234566786786694564678978.434656576768342553566");

    LongFraction t8("324664521234566.786786694564678978434656576768342553566", 100, 10);
    assert(t8.print() == "324664521234566.786786694564");
    std::cout << "Test Constructor... OK" << std::endl;
}

void test_compare() {
    std::cout << "Test Compare..." << std::endl;

    LongFraction t1("324664521234566.786786694564");
    LongFraction t2("324664521234566.786786694564");
    assert(t1 == t2);

    LongFraction t3("324664521234566.786786694564");
    LongFraction t4("1324664521234566.786786694564");
    assert(t3 < t4);

    LongFraction t5("324664521234566.786786694564");
    LongFraction t6("424664521234566.786786694564");
    assert(t5 < t6);

    LongFraction t7("-324664521234566.786786694564");
    LongFraction t8("-424664521234566.786786694564");
    assert(t7 > t8);

    LongFraction t9("32466.784564");
    LongFraction t10("-424664521234566.786786694564");
    assert(t10 < t9);

    std::cout << "Test Compare... OK" << std::endl;
}

void test_unary_minus() {
    std::cout << "Test Unary Minus..." << std::endl;

    LongFraction t1("324664521234566.786786694564");
    LongFraction t2("-324664521234566.786786694564");
    assert(t1 == -t2);

    LongFraction t3("-324664521234566.786786694564");
    LongFraction t4("324664521234566.786786694564");
    assert(t3 == -t4);

    std::cout << "Test Unary Minus... OK" << std::endl;
}

void test_abs() {
    std::cout << "Test Abs..." << std::endl;

    LongFraction t1("324664521234566.786786694564");
    LongFraction t2("-324664521234566.786786694564");
    assert(t1 == t2.abs());

    LongFraction t3("-324664521234566.786786694564");
    LongFraction t4("324664521234566.786786694564");
    assert(t3.abs() == t4);

    std::cout << "Test Abs... OK" << std::endl;
}

void test_summa() {
    std::cout << "Test Summa..." << std::endl;

    LongFraction t1("111111111111111111111111111111");
    LongFraction t2("333333333333333333333333333333");
    LongFraction t3 = t1 + t2;
    assert(t3.print() == "444444444444444444444444444444.0");

    LongFraction t4("111111111111111111111111111111.111111111");
    LongFraction t5("333333333333333333333333333333.333333333");
    LongFraction t6 = t4 + t5;
    assert(t6.print() == "444444444444444444444444444444.444444444");

    LongFraction t7("-111111111111111111111111111111");
    LongFraction t8("-333333333333333333333333333333");
    LongFraction t9 = t7 + t8;
    assert(t9.print() == "-444444444444444444444444444444.0");

    LongFraction t10("-111111111111111111111111111111.111111111");
    LongFraction t11("-333333333333333333333333333333.333333333");
    LongFraction t12 = t10 + t11;
    assert(t12.print() == "-444444444444444444444444444444.444444444");

    LongFraction t13;
    LongFraction t14("11111111.11111111");
    LongFraction t15 = t13 + t14;
    assert(t15.print() == t14.print());

    LongFraction t16("999999.9999999999");
    LongFraction t17("0.0000000001");
    LongFraction t18 = t16 + t17;
    assert(t18.print() == "1000000.0");

    LongFraction t19("999999.99999999");
    LongFraction t20("0.00000001");
    LongFraction t21 = t19 + t20;
    assert(t21.print() == "1000000.0");

    LongFraction y;
    y++;
    assert(y.print() == "1.0");

    LongFraction t22("-999999.9999999999");
    LongFraction t23("-0.0000000001");
    LongFraction t24 = t22 + t23;
    assert((t22 + t23).print() == "-1000000.0");



    std::cout << "Test Summa... OK" << std::endl;
}

void test_diff() {
    std::cout << "Test Diff..." << std::endl;

    LongFraction t1("111111111111111111111111111111");
    LongFraction t2 = t1;
    LongFraction t3 = t1 - t2;
    assert(t3.print() == "0.0");

    LongFraction t4("111111111111111111111111111111");
    LongFraction t5("333333333333333333333333333333");
    assert((t5 - t4).print() == "222222222222222222222222222222.0");
    assert((t4 - t5).print() == "-222222222222222222222222222222.0");

    LongFraction t6("111111111111111111111111111111.111111111");
    LongFraction t7("-333333333333333333333333333333.333333333");
    assert((t6 - t7).print() == "444444444444444444444444444444.444444444");
    assert((t7 - t6).print() == "-444444444444444444444444444444.444444444");

    LongFraction t8("1000000");
    LongFraction t9("0.0000000001");
    assert((t8 - t9).print() == "999999.9999999999");

    std::cout << "Test Diff... OK" << std::endl;
}

void test_mult() {
    std::cout << "Test Product..." << std::endl;
    LongFraction t1;
    LongFraction t2("1111111111111.111111111");
    assert((t1 * t2).print() == "0.0");

    LongFraction t3(2);
    std::cout << (t2 * t3).print() << std::endl;
    assert((t2 * t3).print() == "2222222222222.222222222");
    assert((t2 * -t3).print() == "-2222222222222.222222222");

    LongFraction t4("0.2");
    std::cout << (t2 * t4).print() << std::endl;
    assert((t2 * t4).print() == "222222222222.2222222222");
    assert((t2 * -t4).print() == "-222222222222.2222222222");

    std::cout << "Test Product... OK" << std::endl;
}



void main_test()
{
    test_constructor();
    test_compare();
    test_unary_minus();
    test_abs();
    test_summa();
    test_diff();
    test_mult();
}
