
#ifndef BIG_INT_H
#define BIG_INT_H

#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include <algorithm>
#include <iostream>
#include <cstring>

#include <wiz/global.h>


// type? - long long?
namespace wiz {
	const std::vector<long long> big_int_zero_int(1, 0); /// ZERO_INT
	const std::vector<long long> big_int_ndef_int; ///  empty state..

	class big_int { // namespace  to  class // private:
	public:
		static const long long BIGINT_BASE = (long long)1000000000; /// chk!!  changable?
		static const long long BIGINT_DIGIT_NUM = 9;
													  // const std::vector<long long> one_int( 1, '1' );
		static std::vector<long long> remove_first_zeros(const std::vector<long long>& x) /// remove first all zeros.
		{
			int zero_num = 0;

			for (int i = 0; i < x.size(); i++)
			{
				if (x[i] != 0) { break; }
				zero_num++;
			}
			if (0 == zero_num) { return x; }
			if (x.size() == zero_num) { return big_int_ndef_int; } /// chk...
			std::vector<long long> temp(x.size() - zero_num);
			for (int i = 0; i < temp.size(); i++)
			{
				temp[i] = x[i + zero_num];
			}

			return temp;
		}

		static bool IsSameValues(const std::vector<long long>& arr1, const std::vector<long long>& arr2)
		{
			std::vector<long long> _arr1 = remove_first_zeros(arr1);
			std::vector<long long> _arr2 = remove_first_zeros(arr2);

			if (_arr1.size() != _arr2.size()) { return false; }
			for (int i = 0; i < _arr1.size(); i++)
			{
				if (_arr1[i] != _arr2[i]) { return false; }
			}
			return true;
		}

		/// Less Than  < 
		static bool COMP_LT(const std::vector<long long>& x, const std::vector<long long>& y)
		{
			std::vector<long long> _x = remove_first_zeros(x);
			std::vector<long long> _y = remove_first_zeros(y);

			if (_x.size() > _y.size()) { return false; }
			if (_x.size() < _y.size()) { return true; }
			if (_x.empty()) // also y.empty..
			{
				return false; /// chk?
			}
			for (int i = 0; i < _x.size(); i++)
			{
				if (_x[i] > _y[i]) { return false; }
				if (_x[i] < _y[i]) { return true; }
			}
			// same...
			return false;
		}
		/// greater than >
		static bool COMP_GT(const std::vector<long long>& x, const std::vector<long long>& y)
		{
			return COMP_LT(y, x);
		}
		static std::vector<long long> add_last_zeros(const std::vector<long long>& x, const int n)
		{
			if (x.size() + n <= 0) { return big_int_ndef_int; }
			std::vector<long long> temp(x.size() + n, 0);
			for (int i = 0; i < x.size(); i++)
			{
				temp[i] = x[i];
			}
			return temp;
		}

		static std::vector<long long> _int_plus(const std::vector<long long>& x, const std::vector<long long>& y)
		{
			// err-> return empty array of long long.
			if (x.empty() || y.empty()) { return big_int_ndef_int; }
			std::vector<long long> _x = remove_first_zeros(x);
			std::vector<long long> _y = remove_first_zeros(y);
			if (_x.empty()) { _x = big_int_zero_int; }
			if (_y.empty()) { _y = big_int_zero_int; }

			const int _max = std::max(_x.size(), _y.size());
			const int _min = std::min(_x.size(), _y.size());
			std::vector<long long> temp(_max + 1, 0);
			long long itemp = 0;
			int w = _x.size() - 1;
			int v = _y.size() - 1;
			int u = temp.size() - 1;


			for (int num = 0; num < _min; num++)
			{
				long long sum = (_x[w]) + (_y[v]) + itemp;
				temp[u] = (sum % BIGINT_BASE);
				itemp = sum / BIGINT_BASE;
				w--;
				v--;
				u--;
			}
			for (; w >= 0; w--)
			{
				long long sum = (_x[w]) + itemp;
				temp[u] = (sum % BIGINT_BASE);
				itemp = sum / BIGINT_BASE;
				u--;
			}
			for (; v >= 0; v--)
			{
				long long sum = (_y[v]) + itemp;
				temp[u] = (sum % BIGINT_BASE);
				itemp = sum / BIGINT_BASE;
				u--;
			}
			temp[0] = (itemp);

			std::vector<long long> ret = remove_first_zeros(temp);
			if (ret.empty()) { return big_int_zero_int; }
			return ret;
		}
		static std::vector<long long> _int_minus(const std::vector<long long>& x, const std::vector<long long>& y) // x >= y
		{
			if (x.empty() || y.empty()) { return big_int_ndef_int; }
			std::vector<long long> _x = remove_first_zeros(x);
			std::vector<long long> _y = remove_first_zeros(y);
			if (_x.empty()) { _x = big_int_zero_int; }
			if (_y.empty()) { _y = big_int_zero_int; }

			if (IsSameValues(_x, _y)) { return big_int_zero_int; }
			if (COMP_LT(_x, _y)) { throw 1; } /// wiz::Error(__FILE__, __LINE__, " in int_minus, must be x >= y, but now x < y. "); }


			std::vector<long long> temp(_x.size(), 0);
			const int min = _y.size();
			int w = _x.size() - 1;
			int v = _y.size() - 1;
			int u = temp.size() - 1;
			long long itemp = 0;

			for (int num = 0; num < min; num++)
			{
				long long dif = (_x[w]) - (_y[v]);
				if (dif < 0) /// chk!!
				{
					dif = dif + BIGINT_BASE;
					long long count = w - 1;
					while (_x[count] == 0) {
						_x[count] = (BIGINT_BASE - 1);
						count--;
					}
					_x[count]--;
				}
				temp[u] = (dif % BIGINT_BASE);

				w--;
				v--;
				u--;
			}
			//
			for (; w >= 0; w--)
			{
				long long dif = (_x[w]);
				temp[u] = (dif % BIGINT_BASE);
				u--;
			}

			std::vector<long long> ret = remove_first_zeros(temp);
			if (ret.empty()) { return big_int_zero_int; }
			return ret;
		}
		static std::vector<long long> _int_multiple(const std::vector<long long>& x, const long long ch)
		{
			if (x.empty()) { return big_int_ndef_int; }
			std::vector<long long> _x = remove_first_zeros(x);
			if (_x.empty()) { _x = big_int_zero_int; }

			std::vector<long long> temp(_x.size() + 1, 0);
			long long itemp = 0;

			for (int i = temp.size() - 1; i >= 1; i--)
			{
				long long sum = (ch) * (_x[i - 1]) + itemp;
				temp[i] = (sum % BIGINT_BASE);
				itemp = sum / BIGINT_BASE;
			}
			temp[0] = (itemp);
			std::vector<long long> ret = remove_first_zeros(temp);
			if (ret.empty()) { return big_int_zero_int; }
			return ret;
		}
		static std::vector<long long> _int_multiple(const std::vector<long long>& x, const std::vector<long long>& y)
		{
			if (x.empty() || y.empty()) { return big_int_ndef_int; }
			std::vector<long long> _x = remove_first_zeros(x);
			std::vector<long long> _y = remove_first_zeros(y);
			if (_x.empty()) { _x = big_int_zero_int; }
			if (_y.empty()) { _y = big_int_zero_int; }

			std::vector<long long> sum = big_int_zero_int;

			long long w = 0;
			for (int i = _y.size() - 1; i >= 0; i--) {
				std::vector<long long> temp = add_last_zeros(_int_multiple(_x, _y[i]), w);
				if (temp.empty()) {
					temp = big_int_zero_int;
				}
				std::vector<long long> temp2 = _int_plus(sum, temp);
				sum.clear();
				sum = temp2;
				w++;
			}

			std::vector<long long> ret = remove_first_zeros(sum);
			if (ret.empty()) {
				return big_int_zero_int;
			}
			return ret;
		}
		static std::vector<long long> _int_sub(const std::vector<long long>& x, const int start, const int end)
		{
			// chk start <= end!
			std::vector<long long> temp(end - start + 1);
			for (int i = 0; i < temp.size(); i++)
			{
				temp[i] = x[i + start];
			}

			return temp;
		}

		static std::vector<long long> _int_concat(const std::vector<long long>& x, const std::vector<long long>& y)
		{
			if (x.empty() && y.empty()) { return big_int_ndef_int; }
			std::vector<long long> temp(x.size() + y.size());
			int count = 0;
			for (int i = 0; i < x.size(); i++)
			{
				temp[count] = x[i];
				count++;
			}
			for (int i = 0; i < y.size(); i++)
			{
				temp[count] = y[i];
				count++;
			}

			return temp;
		}

		/// chk this...
		static bool _int_divide(const std::vector<long long>& x, const std::vector<long long>& y,
			std::vector<long long>& quotient, // ¸ò
			std::vector<long long>& remainder) // ³ª¸ÓÁö
		{
			if (x.empty() || y.empty()) { return false; } /// chk???
			std::vector<long long> _x = remove_first_zeros(x);
			const std::vector<long long> _y = remove_first_zeros(y);

			if (_y.empty()) { throw 2; } // return false? or wiz::Error(__FILE__, __LINE__, "_int_divide, x/0 error"); }
			if (_x.empty()) { _x = big_int_zero_int; }
			if (COMP_LT(_x, _y)) {
				quotient.clear(); quotient = big_int_zero_int;
				remainder.clear();
				remainder = _x;
				return true;
			}

			//
			std::vector<long long> vec_quo;
			int k = 0;
			int m = 0;
			std::vector<long long> itemp;
			std::vector<long long> val;

			while (true) {
				if (m >= _x.size()) { break; }
				const std::vector<long long> temp_concat = _int_concat(itemp, _int_sub(_x, m, k));
				if (COMP_LT(temp_concat, _y))
				{
					vec_quo.push_back((0));
					k++;
					if (IsSameValues(big_int_zero_int, temp_concat))
					{
						m++; k = m;
						itemp.clear();
					}
					else if (k == _x.size()) {
						itemp.clear();
						itemp = temp_concat;
						break;
					}
				}
				else
				{
					long long left = 0; long long right = (BIGINT_BASE - 1);
					long long middle = (left + right) >> 1; // 
															//
					while (left <= right) {
						middle = (left + right) >> 1; /// / 2

						val.clear();
						const std::vector<long long> llvec = _int_multiple(_y, middle);
						if (COMP_LT(llvec, temp_concat) || IsSameValues(llvec, temp_concat)) {
							val = _int_minus(temp_concat, llvec);
						}
						else {
							right = middle - 1;
							continue;
						}

						if (COMP_LT(val, _y) && (IsSameValues(big_int_zero_int, val) || COMP_LT(big_int_zero_int, val))) {
							vec_quo.push_back(middle);//
							itemp.clear();
							itemp = remove_first_zeros(val);
							break;
						}
						else if (COMP_LT(_y, val) || IsSameValues(val, _y)) {
							left = middle + 1;
						}
						else
						{
							right = middle - 1;
						}
					}
					m = k + 1;
					k = m;
				}
			}

			// return
			quotient.clear(); quotient = remove_first_zeros(vec_quo);
			if (quotient.empty()) { quotient = big_int_zero_int; }
			remainder.clear();
			if (itemp.empty()) { remainder = big_int_zero_int; }
			else remainder = itemp;
			return true;
		}

	public:
		class BigInt /// class : long Int...
		{
		private:
			std::string remove_first_zeros(const std::string& str)
			{
				std::string temp;
				int state = 0;

				for (int i = 0; i < str.size(); ++i) {
					if (str[i] == '0' && state == 0) {

					}
					else if (str[i] != '0') {
						state = 1;
					}

					if (state == 1)
					{
						temp.push_back(str[i]);
					}
				}
				if (temp.empty()) { return "0"; }
				return temp;
			}
			bool IsInteger(const std::string& str) {
				int state = 0;
				for (int i = 0; i < str.size(); ++i) {
					switch (state)
					{
					case 0:
						if ('+' == str[i] || '-' == str[i]) {
							state = 0;
						}
						else if (str[i] >= '0' && str[i] <= '9')
						{
							state = 1;
						}
						else return false;
						break;
					case 1:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 1;
						}
						else return false;
					}
				}
				return 1 == state;
			}
		public:
			std::vector<long long> val; /// absolute value!
			bool sign; /// +(true), -(false)
		public:
			explicit BigInt() : sign(true)
			{
			}

			explicit BigInt(const std::vector<long long>& val, const bool& sign)
				: val(val), sign(sign)
			{
			}
			BigInt(const BigInt& other)
			{
				val = other.val;
				sign = other.sign;
			}
			BigInt(const long long number)
			{
				/// chk size of number!
				/// to string -> toBigInt
				std::string str = wiz::toStr2(number, 9); /// remove first zeros?
				str = remove_first_zeros(str); /// need for test!!
				(*this) = BigInt(str);
			}
			BigInt(const std::string& str)
			{
				if (false == IsInteger(str)) { return; }
				/// str is not integer type, then err!?

				bool isMinus = false;
				int size = 0;
				char* arr = NULL;
				if (str[0] == '-') {
					sign = false;
					isMinus = true;
					arr = new char[str.size()];
					size = str.size() - 1;
					strcpy(arr, str.c_str() + 1);
				}
				else {
					sign = true;
					arr = new char[str.size() + 1];
					strcpy(arr, str.c_str());
					size = str.size();
				}

				while (true)
				{
					/// todo - fix!!
					size = std::max(size - 9, 0);
					val.push_back(atoll(arr + size));

					if (size == 0) {
						break;
					}
					arr[size] = '\0';
				}

				delete[] arr;
				std::reverse(val.begin(), val.end());
			}

			BigInt& operator=(const BigInt& longInt)
			{
				BigInt temp = longInt;
				val.clear();
				val = std::move(temp.val);
				sign = temp.sign;

				return *this;
			}
			BigInt& operator=(BigInt&& other)
			{
				this->val = std::move(other.val);
				std::swap(this->sign, other.sign);
				return *this;
			}
		public:
			friend bool operator!=(const BigInt& num1, const BigInt& num2)
			{
				return !IsSameValues((num1 - num2).val, big_int_zero_int);
			}
			friend bool operator<(const BigInt& num1, const BigInt& num2)
			{
				return (num1 - num2).sign == false;
			}
			friend bool operator>(const BigInt& num1, const BigInt& num2)
			{
				return num2 < num1;
			}
			friend bool operator==(const BigInt& num1, const BigInt& num2)
			{
				return IsSameValues((num1 - num2).val, big_int_zero_int);
			}
			friend bool operator<=(const BigInt& num1, const BigInt& num2)
			{
				return num1 < num2 || num1 == num2;
			}
			friend bool operator>=(const BigInt& num1, const BigInt& num2)
			{
				return num1 > num2 || num1 == num2;
			}
			friend BigInt operator+(const BigInt& num1, const BigInt& num2)
			{
				if (num1.val.empty() || num2.val.empty())
				{
					throw 3; // wiz::Error(__FILE__, __LINE__, "empty problem, in + ");
				}
				BigInt number;

				if (num1.sign == num2.sign) {
					number.val = _int_plus(num1.val, num2.val);
					number.sign = num1.sign;
					if (IsSameValues(big_int_zero_int, number.val)) { number.sign = true; }// think only +0.
				}
				else if (num1.sign && !num2.sign) {
					if (num1.val < num2.val)
					{
						number.val = _int_minus(num2.val, num1.val);
						number.sign = false; // minus
					}
					else if (IsSameValues(num1.val, num2.val))
					{
						number.val = std::vector<long long>(1, 0);
						number.sign = true; // +0 = 0 = -0 ??
					}
					else
					{
						number.val = _int_minus(num1.val, num2.val);
						number.sign = true;
					}
				}
				else if (!num1.sign && num2.sign) {
					if (num1.val < num2.val)
					{
						number.val = _int_minus(num2.val, num1.val);
						number.sign = true; // minus
					}
					else if (IsSameValues(num1.val, num2.val))
					{
						number.val = std::vector<long long>(1, (0));
						number.sign = true; // +0 = 0 = -0 ??
					}
					else
					{
						number.val = _int_minus(num1.val, num2.val);
						number.sign = false;
					}
				}
				return number;
			}
			friend BigInt operator-(const BigInt& num1, const BigInt& num2)
			{
				if (num1.val.empty() || num2.val.empty())
				{
					throw 4; // wiz::Error(__FILE__, __LINE__, "empty problem, in - ");
				}
				BigInt number = num2;
				number.sign = !number.sign;

				return num1 + number;
			}
			friend BigInt operator*(const BigInt& num1, const BigInt& num2)
			{
				if (num1.val.empty() || num2.val.empty())
				{
					throw 5; // wiz::Error(__FILE__, __LINE__, "empty problem, in * ");
				}
				BigInt temp;
				temp.val = _int_multiple(num1.val, num2.val);
				temp.sign = (num1.sign == num2.sign);

				return temp;
			}
			friend BigInt operator/(const BigInt& num1, const BigInt& num2)
			{
				if (num1.val.empty() || num2.val.empty())
				{
					throw 6; // wiz::Error(__FILE__, __LINE__, "empty problem, in / ");
				}
				BigInt temp;
				std::vector<long long> quotient;
				std::vector<long long> remainder;

				_int_divide(num1.val, num2.val, quotient, remainder);

				temp.val = quotient;
				temp.sign = (num1.sign == num2.sign); /// chk...

				return temp;
			}
			friend BigInt operator%(const BigInt& num1, const BigInt& num2)
			{
				if (num1.val.empty() || num2.val.empty())
				{
					throw 7; // wiz::Error(__FILE__, __LINE__, "empty problem, in % ");
				}
				if (!num1.sign || !num2.sign) /// chk...
				{
					throw 8; // wiz::Error(__FILE__, __LINE__, "BigInt%, sign is minus");
				}

				BigInt temp;
				std::vector<long long> quotient;
				std::vector<long long> remainder;

				_int_divide(num1.val, num2.val, quotient, remainder);

				temp.val = remainder;
				temp.sign = true;


				return temp;
			}
		public:
			std::string ToString()const
			{
				string temp;

				if (this->sign == false) {
					temp = "-";
				}
				temp = temp + wiz::toStr(this->val[0]);
				for (int i = 1; i < this->val.size(); ++i) {
					temp = temp + wiz::toStr2(this->val[i], 9);
				}
				return temp;
			}
			friend
				ostream& operator<<(ostream& stream, const BigInt& bigInt)
			{
				stream << bigInt.ToString();
				return stream;
			}
		};
	};
}

#endif

