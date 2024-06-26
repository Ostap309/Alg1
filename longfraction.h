#ifndef LONGFRACTION_H
#define LONGFRACTION_H

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>

typedef unsigned int uint;

class LongFraction
{
public:
    LongFraction(const uint base = 100000, const uint precision = 100);
    LongFraction(int value, const uint base = 100000, const uint precision = 100);
    LongFraction(const std::string number, const uint base = 100000, const uint precision = 100);
    LongFraction(const LongFraction& other);

    LongFraction operator=(const LongFraction other);

    bool operator ==(const LongFraction other) const;
    bool operator !=(const LongFraction other) const;
    bool operator <(const LongFraction other) const;
    bool operator <=(const LongFraction other) const;
    bool operator >=(const LongFraction other) const;
    bool operator >(const LongFraction other) const;

    std::string print();

    LongFraction operator+(const LongFraction other) const;
    void operator++(int);

    LongFraction operator-(const LongFraction other) const;
    LongFraction operator--(int);
    LongFraction operator-();

    LongFraction operator*(const LongFraction other) const;

    LongFraction abs() const;

    ~LongFraction();

private:
    uint _count_bucket() const;
    void _reduce();
    uint _get_precision() const;
    std::string _get_string(const std::vector<uint>) const;
    std::vector<uint> _fill(const std::vector<uint>) const;
    std::vector<uint> _sum_digits(std::vector<uint>, std::vector<uint>) const;
    std::vector<uint> _diff_digits(std::vector<uint>, std::vector<uint>) const;
    std::vector<uint> _mult_digits(std::vector<uint>, std::vector<uint>) const;
    std::vector<uint> _merge_digits(const std::vector<uint>, std::vector<uint>) const;
    std::vector<uint> _mult_digits_uint(const std::vector<uint>, uint) const;
    bool _compare(const std::vector<uint>, const std::vector<uint>)  const;

private:
    std::vector<uint> _digit;
    std::vector<uint> _fraction;
    bool _sign;
    uint _base;
    uint _count;
    uint _precision;
};

#endif
