
#ifndef BIG_FRACTION_H
#define BIG_FRACTION_H

#define _CRT_SECURE_NO_WARNINGS
#include <wiz/big_int.h>

namespace wiz
{
	class big_frarction {
		class BigFraction {
		private:
			bool m; /// is_minus
			wiz::big_int::BigInt a, b; /// a/b/// template UNSIGNED_INT?? for long long?

		private:
			wiz::big_int::BigInt _getGCD(const wiz::big_int::BigInt& x, const wiz::big_int::BigInt& y) const
			{
				wiz::big_int::BigInt a = std::max(x, y);
				wiz::big_int::BigInt b = std::min(x, y);
				wiz::big_int::BigInt c = a % b;

				while (0 != c)
				{
					a = b;
					b = c;
					c = a % b;
				}
				return b;
			}
			wiz::big_int::BigInt _getLCM(const wiz::big_int::BigInt& x, const wiz::big_int::BigInt& y) const
			{
				return (x * y) / (_getGCD(x, y));
			}
		public:
			wiz::big_int::BigInt getA()const {
				return a;
			}
			wiz::big_int::BigInt getB()const {
				return b;
			}
			explicit BigFraction() : a(0), b(1), m(false) { }
			explicit BigFraction(const bool bm, const wiz::big_int::BigInt& x, const wiz::big_int::BigInt& y)
			{
				m = bm;
				a = x;
				b = y;
				if (0 == b) {
					throw "err: b(divider) is zero.";
				}
				if (a == 0 && m == true) { m = false; }
				simple();
			}
			void simple()
			{
				if (a > 1 && b > 1) { /// chk..
					const wiz::big_int::BigInt gcd = _getGCD(a, b);
					a = a / gcd;
					b = b / gcd;
				}
			}
			BigFraction operator+(const BigFraction& frc)const
			{
				if (this->a == 0) { return frc; }
				if (frc.a == 0) { return *this; }

				BigFraction temp1 = *this;
				BigFraction temp2 = frc;
				BigFraction result;
				const wiz::big_int::BigInt lcm = _getLCM(temp1.b, temp2.b);

				///
				temp1.a = temp1.a * lcm / temp1.b;
				temp2.a = temp2.a * lcm / temp2.b;
				temp1.b = lcm;
				temp2.b = lcm;
				result.b = lcm;
				///
				if (temp1.m == temp2.m)
				{
					result.m = temp1.m;
					result.a = temp1.a + temp2.a;
				}
				else if (temp1.a == temp2.a) {
					result.m = false;
					result.a = 0;
					result.b = 1;
				}
				else if (temp1.a > temp2.a)
				{
					result.a = temp1.a - temp2.a; ///
					result.m = temp1.m;
				}
				else
				{
					result.a = temp2.a - temp1.a;
					result.m = temp2.m;
				}

				result.simple();

				return result;
			}
			void operator+=(const BigFraction& frc)
			{
				*this = (*this) + frc;
			}
			BigFraction operator-(const BigFraction& frc)const
			{
				if (0 == frc.a) {
					return *this;
				}
				else if (0 == this->a) {
					return BigFraction(!frc.m, frc.a, frc.b);
				}
				return (*this) + BigFraction(!frc.m, frc.a, frc.b);
			}

			BigFraction operator-() const
			{
				if (0 == a) {
					return BigFraction(false, 0, 1);
				}
				else {
					return BigFraction(!m, a, b);
				}
			}
			void operator-=(const BigFraction& frc)
			{
				(*this) = (*this) - frc;
			}
			BigFraction operator*(const BigFraction& frc)const
			{
				if (frc.a == 0) {
					return BigFraction(false, 0, 1);
				}
				if (this->a == 0) {
					return BigFraction(false, 0, 1);
				}
				BigFraction temp1 = *this, temp2 = frc;
				BigFraction result;

				result.m = !(temp1.m == temp2.m);
				result.a = temp1.a * temp2.a;
				result.b = temp1.b * temp2.b;
				if (result.b == 0) { throw "err.: b is zero"; }

				result.simple();

				return result;
			}
			void operator*=(const BigFraction& frc)
			{
				(*this) = (*this) * frc;
			}
			BigFraction operator/(const BigFraction& frc)const
			{
				return (*this) * BigFraction(frc.m, frc.b, frc.a);
			}
			void operator/=(const BigFraction& frc)
			{
				(*this) = (*this) / frc;
			}
			friend ostream& operator<<(ostream& os, const BigFraction& frc)
			{
				// for not class? 
				if (frc.m && 0 != frc.a) {
					os << "-";
				}
				if (1 == frc.b) {
					os << frc.a;
				}
				else if (0 == frc.a) {
					os << "0";
				}
				else {
					os << frc.a << " / " << frc.b;
				}
				return os;
			}
			bool operator==(const BigFraction& frc)const
			{
				return m == frc.m && a == frc.a && b == frc.b;
			}
			bool operator!=(const BigFraction& frc)const
			{
				return !(*this == frc);
			}
			bool isMinusValue() const
			{
				return m;
			}
			bool operator<(const BigFraction& frc)const
			{
				BigFraction temp1 = *this, temp2 = frc;

				BigFraction result = temp1 - temp2;
				return result.isMinusValue();
			}
			bool operator>(const BigFraction& frc)const
			{
				return frc < (*this);
			}
			bool operator>=(const BigFraction& frc)const
			{
				return (*this) > frc || (*this) == frc;
			}
			bool isZero() const
			{
				return 0 == a;
			}
		};
	};
}


#endif
