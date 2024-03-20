
#include "longfraction.h"

LongFraction::LongFraction(const uint base, const uint precision) {
    this->_base = base;
    this->_count = (uint)std::log10(this->_base);
    this->_precision = precision;
    this->_sign = true;
}

LongFraction::LongFraction(int value, const uint base, const uint precision) {
    this->_base = base;
    this->_count = (uint)std::log10(this->_base);
    this->_precision = precision;
    this->_sign = value >= 0 ? true : false;
    value = std::abs(value);
    while (value / base > 0)
    {
        this->_digit.insert(this->_digit.begin(), value % base);
        value = value / base;
    }
    this->_digit.insert(this->_digit.begin(), value % base);
}

LongFraction::LongFraction(std::string number, const uint base, const uint precision) {
    this->_base = base;
    this->_count = (uint)std::log10(this->_base);
    this->_precision = precision;
    this->_sign = true;
    if (number[0] == '-') {
        this->_sign = false;
        number = number.erase(0, 1);
    }
    if (number[0] == '+')
    {
        number = number.erase(0, 1);
    }
    std::string digit;
    if (number.find(".") > number.size())
        digit = number;
    else
        digit = number.substr(0, number.find("."));

    std::string piece = digit.substr(0, digit.size() % this->_count);
    if (piece != "")
        this->_digit.push_back(std::stoi(piece));
    for(size_t i = digit.size() % this->_count; i < digit.size(); i += this->_count) {
        piece = digit.substr(i, this->_count);
        this->_digit.push_back(std::stoi(piece));
    }

    if (number.find(".") > number.size()) return;
    uint count_bucket = this->_count_bucket();
    std::string fraction = number.substr(number.find(".") + 1);
    for(size_t i = 0 ; i < fraction.size(); i += this->_count) {
        std::string piece = fraction.substr(i, this->_count);
        this->_fraction.push_back(std::stoi(piece));
        if (this->_fraction.size() == count_bucket)
            break;
    }
}

LongFraction::LongFraction(const LongFraction &other) {
    this->_base = other._base;
    this->_precision = other._precision;
    this->_sign = other._sign;
    this->_digit = other._digit;
    this->_fraction = other._fraction;
    this->_count = other._count;
}

LongFraction LongFraction::operator=(const LongFraction other) {
    this->_base = other._base;
    this->_precision = other._precision;
    this->_sign = other._sign;
    this->_digit = other._digit;
    this->_fraction = other._fraction;
    this->_count = other._count;
    return *this;
}

bool LongFraction::operator==(const LongFraction other) const {
    return (this->_sign == other._sign) && (this->_base == other._base) &&
            (this->_digit == other._digit) && (this->_fraction == other._fraction);
}

bool LongFraction::operator!=(const LongFraction other) const {
    return !(*this == other);
}

bool LongFraction::operator<(const LongFraction other) const {
    if (!this->_sign && other._sign)
        return true;
    if (this->_sign && !other._sign)
        return false;
    if (this->_sign)
        return this->_compare(this->_digit, other._digit) &&
                this->_compare(this->_fill(this->_fraction), this->_fill(other._fraction));
    else
        return !(this->_compare(this->_digit, other._digit) &&
                 this->_compare(this->_fill(this->_fraction), this->_fill(other._fraction)));
}

bool LongFraction::operator<=(const LongFraction other) const {
    return *this < other || *this == other;
}

bool LongFraction::operator>=(const LongFraction other) const {
    return *this > other || *this == other;
}

bool LongFraction::operator>(const LongFraction other) const {
    return !(*this < other);
}

std::string LongFraction::print()
{
    std::string ss = "";
    if (!this->_sign)
        ss += "-";

    if (this->_digit.empty())
        ss += "0";
    else {
        std::vector<uint>::iterator it = this->_digit.begin();
        ss += std::to_string(*it);
        it++;
        for(; it != this->_digit.end(); it++) {
            std::string s = std::to_string(*it);
            while (s.size() < this->_count)
                s = "0" + s;
            ss += s;
        }
    }

    ss += ".";

    if (this->_fraction.empty())
        ss += "0";
    else {
        std::vector<uint>::iterator it = this->_fraction.begin(), next = it + 1;
        while ( next != this->_fraction.end()) {
            std::string s = std::to_string(*it);
            while (s.size() < this->_count)
                s = "0" + s;
            ss += s;
            it++;
            next++;
        }
        ss += std::to_string(*it);
    }
    return ss;
}

LongFraction LongFraction::operator+(const LongFraction other) const {
    if (!this->_sign && !other._sign)
        return -(this->abs() + other.abs());
    if (this->_sign && !other._sign)
        return *this - other.abs();
    if (!this->_sign && other._sign)
        return other - this->abs();

    std::vector<uint> left = this->_merge_digits(this->_digit, this->_fill(this->_fraction));
    std::vector<uint> right = other._merge_digits(other._digit, other._fill(other._fraction));
    // считаем что левое слагаемое длинее чем правое
    if (left.size() < right.size())
        std::swap(left, right);
    std::vector<uint> res = this->_sum_digits(left, right);
    LongFraction summa(this->_base, this->_precision);
    uint count_bucket = this->_fill(this->_fraction).size();
    size_t count_digit = res.size() - count_bucket;
    size_t i = 0;
    std::vector<uint>::iterator it = res.begin();
    for( ; i < count_digit; i++, it++)
        summa._digit.push_back(*it);
    for( ; it < res.end(); it++)
        summa._fraction.push_back(*it);
    summa._reduce();
    return summa;
}


void LongFraction::operator++(int v) {
    *this = *this + LongFraction(1, this->_base, this->_precision);
}

LongFraction LongFraction::operator-(const LongFraction other) const {
    if (*this == other)
        return LongFraction(this->_base, this->_precision);
    if (this->_sign && ! other._sign)
        return *this + other.abs();
    if (!this->_sign && other._sign)
        return -(this->abs() + other);
    if (!this->_sign && !other._sign)
        return other.abs() - *this;

    if (*this < other)
        return -(other - *this);

    // Осталась ситуация, когда из большего положительного числа вычитаем меньшее
    std::vector<uint> left = this->_merge_digits(this->_digit, this->_fill(this->_fraction));
    std::vector<uint> right = other._merge_digits(other._digit, other._fill(other._fraction));
    std::vector<uint> res = this->_diff_digits(left, right);
    LongFraction diff(this->_base, this->_precision);
    uint count_bucket = this->_fill(this->_fraction).size();
    size_t count_digit = res.size() - count_bucket;
    size_t i = 0;
    std::vector<uint>::iterator it = res.begin();
    for( ; i < count_digit; i++, it++)
        diff._digit.push_back(*it);
    for( ; it < res.end(); it++)
        diff._fraction.push_back(*it);
    diff._reduce();
    return diff;
}

LongFraction LongFraction::operator--(int v) {
    return *this - LongFraction(1, this->_base, this->_precision);
}

LongFraction LongFraction::operator-() {
    this->_sign = this->_sign ? false: true;
    return *this;
}

LongFraction LongFraction::operator*(const LongFraction other) const {
    LongFraction mult(this->_base, this->_precision);
    mult._sign = this->_sign && other._sign;
    if (*this == mult || other == mult)
        return mult;

    std::vector<uint> left = this->_merge_digits(this->_digit, this->_fraction);
    std::vector<uint> right = other._merge_digits(other._digit, other._fraction);
    // считаем что левое слагаемое длинее чем правое
    if (left.size() < right.size())
        std::swap(left, right);

    std::vector<uint> result = this->_mult_digits(left, right);
    if (result.empty())
        return mult;
    uint precision = this->_get_precision() + other._get_precision();
    std::string ss = "";
    std::vector<uint>::const_iterator it = result.begin(), next;
    ss += std::to_string(*it);
    it++; next = it + 1;
    for(; next != result.end(); it++, next++) {
        std::string s = std::to_string(*it);
        while (s.size() < this->_count)
            s = "0" + s;
        ss += s;
    }
    ss += std::to_string(*it);
    ss = ss.substr(0, ss.size() - precision) + "." + ss.substr(ss.size() - precision);
    if (!(this->_sign && other._sign)) ss = "-" + ss;
    return LongFraction(ss, this->_base, this->_precision);
}

LongFraction LongFraction::abs() const {
    LongFraction other(*this);
    other._sign = true;
    return other;
}

LongFraction::~LongFraction() {

}

uint LongFraction::_count_bucket() const {
    return (uint)(this->_precision / this->_count + 1);
}

void LongFraction::_reduce()
{
    if (this->_digit.size() == 0 && this->_fraction.size() == 0)
        return;
    if (this->_digit.size() > 0)
        while (true) {
            if (this->_digit.size() == 0)
                break;
            if (this->_digit[0] == 0)
                this->_digit.erase(this->_digit.begin());
            else
                break;
        }

    if (this->_fraction.size() > 0)
        while (true) {
            if (this->_fraction.size() == 0)
                break;
            int pop = this->_fraction.back();
            this->_fraction.pop_back();
            if (pop != 0) {
                this->_fraction.push_back(pop);
                break;
            }
        }
}

uint LongFraction::_get_precision() const
{
    if (this->_fraction.empty())
        return 0;

    uint value = this->_fraction.back();
    uint size_bucket = this->_fraction.size();
    return (size_bucket - 1) * this->_count + (uint)std::ceil(std::log10(value + 1));
}

std::string LongFraction::_get_string(const std::vector<uint> numbers) const
{
    std::string ss = "";
        std::vector<uint>::const_iterator it = numbers.begin();
        ss += std::to_string(*it);
        it++;
        for(; it != numbers.end(); it++) {
            std::string s = std::to_string(*it);
            while (s.size() < this->_count)
                s = "0" + s;
            ss += s;
        }
}

std::vector<uint> LongFraction::_fill(const std::vector<uint> digit) const {
    size_t count_bucket = this->_count_bucket();
    std::vector<uint> temp = digit;
    while(temp.size() < count_bucket)
        temp.push_back(0);
    return temp;
}

std::vector<uint> LongFraction::_sum_digits(std::vector<uint> left, std::vector<uint> right) const {
    std::vector<uint> result;
    std::vector<uint>::const_reverse_iterator rlt;
    std::vector<uint>::const_reverse_iterator rrt;
    uint shift = 0;
    for(rlt = left.rbegin(), rrt = right.rbegin(); rrt != right.rend(); ++rlt, ++rrt)  {
        uint t = *rlt + *rrt + shift;
        if (std::to_string(t).size() > std::max(std::to_string(*rlt).size(), std::to_string(*rrt).size())) {
            shift = 1;
            std::string s = std::to_string(t);
            t = std::stoi(s.erase(0, 1));
        }
        else {
            shift = 0;
        }
        result.insert(result.begin(), t);
    }
    for(; rlt != left.rend(); ++rlt) {
        uint t = *rlt + shift;
        result.insert(result.begin(), t % this->_base);
        shift = t / this->_base;
    }
    if (shift != 0)
        result.insert(result.begin(), shift);
    return result;
}

std::vector<uint> LongFraction::_diff_digits(std::vector<uint> left, std::vector<uint> right) const {
    std::vector<uint> result;
    std::vector<uint>::reverse_iterator rlt;
    std::vector<uint>::reverse_iterator rrt;
    uint shift = 0;
    for(rlt = left.rbegin(), rrt = right.rbegin(); rrt != right.rend(); ++rlt, ++rrt)  {
        if (*rlt >= *rrt + shift) {
            result.insert(result.begin(), *rlt - shift - *rrt);
            shift = 0;
        }
        else {
            uint value = this->_base + *rlt - shift - *rrt;
            result.insert(result.begin(), value % this->_base);
            shift = 1;
        }
    }
    for(; rlt != left.rend(); ++rlt) {
        uint t = *rlt - shift;
        if (t >= 0) {
            result.insert(result.begin(), t % this->_base);
            shift = 0;
        }
        else {
            uint value = shift * this->_base + *rlt - shift;
            result.insert(result.begin(), value % this->_base);
            shift = 1;
        }
    }
    return result;
}

std::vector<uint> LongFraction::_mult_digits(std::vector<uint> left, std::vector<uint> right) const {
    std::vector<uint> result;

    if (left.empty())
        return result;

    std::vector<uint>::reverse_iterator rlt;
    std::vector<uint>::reverse_iterator rrt;
    uint count = 0;
    for(rlt = left.rbegin(), rrt = right.rbegin(); rrt != right.rend(); ++rlt, ++rrt)  {
        std::vector<uint> t =  this->_mult_digits_uint(left, *rrt);
        for(uint i = 0; i < count; i++)
            t.push_back(0);
        count++;
        result = this->_sum_digits(t, result);
    }
    return result;
}

std::vector<uint> LongFraction::_merge_digits(std::vector<uint> left, std::vector<uint> right) const {
    std::vector<uint> temp = left;
    temp.insert(temp.end(), right.begin(), right.end());
    return temp;
}

std::vector<uint> LongFraction::_mult_digits_uint(const std::vector<uint> left, uint right) const
{
    if (left.empty())
        return left;
    std::vector<uint> temp;
    uint shift = 0;
    std::vector<uint>::const_reverse_iterator rlit = left.rbegin();
    for(; rlit != left.rend(); rlit++) {
        uint value = right * *rlit + shift;
        temp.insert(temp.begin(), value % this->_base);
        shift = value / this->_base;
    }
    return temp;
}

bool LongFraction::_compare(const std::vector<uint> left, const std::vector<uint> right) const {
    if (left.size() != right.size())
        return left.size() < right.size();

    std::vector<uint>::const_reverse_iterator rlit = left.rbegin();
    std::vector<uint>::const_reverse_iterator rrit = right.rbegin();
    for( ; rlit != left.rend(); rlit++, rrit++)
        if (*rlit > *rrit)
            return false;
    return true;
}
