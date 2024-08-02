#include <iostream>
#include <cstring>
#define MAX_SIZE 10000
using namespace std;
class BigNum{
public:
	BigNum();
	BigNum(int x);
	int size(void);
	short get_num(int idx);
	short set_num(int idx, short num);
	void add_num(int idx, short num);
	BigNum operator+(BigNum y);
	BigNum operator-(BigNum y);
	BigNum operator*(BigNum y);
	BigNum operator/(BigNum y);
	BigNum operator%(BigNum y);
	BigNum operator<<(BigNum y);
	BigNum operator>>(BigNum y);
	/*
	BigNum operator&(BigNum y);
	BigNum operator|(BigNum y);
	BigNum operator!(void);
	*/
	BigNum operator+=(BigNum y);
	BigNum operator-=(BigNum y);
	BigNum operator*=(BigNum y);
	BigNum operator/=(BigNum y);
	BigNum operator%=(BigNum y);
	BigNum operator<<=(BigNum y);
	BigNum operator>>=(BigNum y);
	bool operator<(BigNum y);
	bool operator==(BigNum y);
	bool operator>(BigNum y);
	bool operator<=(BigNum y);
	bool operator>=(BigNum y);
	void output(void);
private:
	char _data[MAX_SIZE+5];
	int _size=0;
};

BigNum::BigNum(){
	memset(_data, 0, sizeof _data);
}
BigNum::BigNum(int x) {
	memset(_data, 0, sizeof _data);
	int i = 1;
	while (x) {
		set_num(i, x % 10);
		x /= 10;
		i++;
	}
}

int BigNum::size() {
	return this->_size;
}

short BigNum::get_num(int idx) {
	if (idx >= MAX_SIZE * 2 - 1) {
		cout << "Out of range!\n";
		return 0;
	}
	if (idx > this->size()) {
		return 0;
	}
	int dit = idx / 2 - (idx % 2 == 0 ? 1 : 0);
	return (idx % 2 == 0 ? _data[dit] / 10 % 10 : _data[dit] % 10);
}

short BigNum::set_num(int idx, short num) {
	if (idx >= MAX_SIZE * 2 - 1) {
		cout << "Out of range!\n";
		return 0;
	}
	if (idx > this->size()) {
		_size = idx;
	}
	if (num >= 10) {
		throw 1;
	}
	int dit = idx / 2 - (idx % 2 == 0 ? 1 : 0);
	_data[dit] -= (idx % 2 == 0 ? _data[dit] / 10 * 10 : _data[dit] % 10);
	_data[dit] += (idx % 2 == 0 ? num * 10 : num);
	return num;
}

void BigNum::add_num(int idx, short num) {
	if (num >= 10) {
		throw 1;
	}
	if (idx > this->size()) {
		_size = idx;
	}
	short tmp = get_num(idx)+num;
	while (tmp >= 10) {
		set_num(idx, tmp % 10);
		tmp /= 10;
		idx++;
		tmp += get_num(idx);
	}
	set_num(idx, tmp);
	_size = (this->size() < idx ? idx : this->size());
}
BigNum BigNum::operator+(BigNum y) {
	BigNum ans=*this;
	for (int i = 1;i <= y.size();i++) {
		ans.add_num(i, y.get_num(i));
	}
	return ans;
}

BigNum BigNum::operator-(BigNum y) {
	BigNum ans = *this;
	if (ans < y) {
		cout << "This function is unsigned.";
		throw 1;
	}
	bool flg = 0;
	for (int i = 1;i <= y.size();i++) {
		short tmp = ans.get_num(i)-y.get_num(i)-flg;
		if (tmp < 0) {
			tmp += 10;
			flg = 1;
		}else {
			flg = 0;
		}
		ans.set_num(i, tmp);
	}
	if (flg)ans.set_num(y.size() + 1, ans.get_num(y.size() + 1) - 1);
	while (ans.get_num(ans.size()) == 0) {
		ans._size--;
	}
	return ans;
}

BigNum BigNum::operator*(BigNum y) {
	BigNum x = *this, ans;
	if (x < y) {
		return y.operator*(x);
	}
	BigNum pre_run[10];
	for (int i = 1;i < 10;i++) {
		pre_run[i] = pre_run[i - 1] + x;
	}
	for (int i = 1;i <= y.size();i++) {
		for (int j = 1;j <= pre_run[y.get_num(i)].size();j++) {
			ans.add_num(i + j - 1, pre_run[y.get_num(i)].get_num(j));
		}
	}
	return ans;
}

BigNum BigNum::operator/(BigNum y) {
	BigNum x = *this;
	BigNum ans, tmp;
	BigNum pre_run[10];
	for (int i = 1;i < 10;i++) {
		pre_run[i] = pre_run[i - 1] + y;
	}
	for (int i = x.size();i >= x.size() - y.size() + 1;i--) {
		tmp = tmp * BigNum(10);
		tmp = tmp + x.get_num(i);
	}
	for (int i = x.size() - y.size() + 1;i >= 1;i--) {
		if (i != x.size() - y.size() + 1) {
			tmp = tmp * BigNum(10);
			tmp = tmp + x.get_num(i);
		}
		int l=0, r=9, mid;
		while(true){
			mid = (l + r) >> 1;
			if (tmp < pre_run[mid + 1] && pre_run[mid] <= tmp) {
				if(!(mid==0))ans.set_num(i, mid+(tmp==0));
				tmp = tmp - pre_run[mid];
				break;
			}
			if (pre_run[mid + 1] <= tmp) {
				l = mid;
			}
			if (pre_run[mid] > tmp) {
				r = mid;
			}
		}
	}
	return ans;
}

BigNum BigNum::operator%(BigNum y) {
	BigNum x = *this;
	BigNum tmp;
	BigNum pre_run[10];
	for (int i = 1;i < 10;i++) {
		pre_run[i] = pre_run[i - 1] + y;
	}
	for (int i = x.size();i >= x.size() - y.size() + 1;i--) {
		tmp = tmp * BigNum(10);
		tmp = tmp + x.get_num(i);
	}
	for (int i = x.size() - y.size() + 1;i >= 1;i--) {
		if (i != x.size() - y.size() + 1) {
			tmp = tmp * BigNum(10);
			tmp = tmp + x.get_num(i);
		}
		int l = 0, r = 9, mid;
		while(true){
			mid = (l + r) >> 1;
			if (tmp < pre_run[mid + 1] && pre_run[mid] <= tmp) {
				tmp = tmp - pre_run[mid];
				break;
			}
			if (pre_run[mid + 1] <= tmp) {
				l = mid;
			}
			if (pre_run[mid] > tmp) {
				r = mid;
			}
		}
	}
	return tmp;
}

BigNum BigNum::operator<<(BigNum y) {
	BigNum t = pow(2, y);
	return this->operator*(t);
}

BigNum BigNum::operator>>(BigNum y) {
	BigNum t = pow(2, y);
	return this->operator/(t);
}


BigNum BigNum::operator+=(BigNum y) {
	return (*this) = this->operator+(y);
}

BigNum BigNum::operator-=(BigNum y) {
	return (*this) = this->operator-(y);
}

BigNum BigNum::operator*=(BigNum y) {
	return (*this) = this->operator*(y);
}

BigNum BigNum::operator/=(BigNum y) {
	return (*this) = this->operator/(y);
}

BigNum BigNum::operator%=(BigNum y) {
	return (*this) = this->operator%(y);
}

BigNum BigNum::operator<<=(BigNum y) {
	return (*this) = this->operator<<(y);
}

BigNum BigNum::operator>>=(BigNum y) {
	return (*this) = this->operator>>(y);
}

bool BigNum::operator<(BigNum y) {
	BigNum x = *this;
	if (x.size() < y.size())return true;
	for (int i = x.size();i >= 1;i--) {
		if (x.get_num(i) != y.get_num(i)) {
			return x.get_num(i) < y.get_num(i);
		}
	}
	return false;
}

bool BigNum::operator==(BigNum y) {
	BigNum x = *this;
	if (x.size() != y.size())return false;
	for (int i = x.size();i >= 1;i--) {
		if (x.get_num(i) != y.get_num(i)) {
			return false;
		}
	}
	return true;
}

bool BigNum::operator>(BigNum y) {
	BigNum x = *this;
	if (x.size() > y.size())return true;
	for (int i = x.size();i >= 1;i--) {
		if (x.get_num(i) != y.get_num(i)) {
			return x.get_num(i) > y.get_num(i);
		}
	}
	return false;
}

bool BigNum::operator<=(BigNum y) {
	return this->operator<(y) || this->operator==(y);
}

void BigNum::output() {
	for (int i = this->size();i >= 1;i--) {
		cout << this->get_num(i);
	}
}

BigNum pow(BigNum x, BigNum y) {
	//y = y * BigNum(2);
	BigNum ans(1);
	while (y > 0) {
		if (y % 2 == 1) {
			ans = ans * x;
		}
		x = x * x;
		y = y / 2;
	}
	return ans;
}
