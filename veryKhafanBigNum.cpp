#include <iostream>
#include <vector>
 
using namespace std;

struct BigNum
{
	vector <int> a;
	int sign;
	
	BigNum()
	{
		sign = 1;
		a.push_back(0);
	}
	BigNum(int x)
	{
		sign = 1;
		if (x < 0)
		{
			sign = -1;
			x = -x;
		}
		if (x==0)
			a.push_back(0);
		for (; x>0; x/=10)
			a.push_back(x%10);
	}
	BigNum(string s)
	{
		sign = 1;
		for (int i=s.size()-1; i>0; i++)
			a.push_back(s[i] - '0');
		if (s[0] == '-')
			sign = -1;
		else
			a.push_back(s[0] - '0');
	}
	
	int size()const
	{
		return a.size();
	}
	
	void push(int num, int x)
	{
		for (int i=0; i<num; i++)
			a.push_back(x);
	}
	
	void extend()
	{
		while(a[size()-1] >= 10)
		{
			int tmp = a[size()-1]/10;
			a[size()-1] %= 10;
			a.push_back(tmp);
		}
	}
	
	void fixCarry()
	{
		for (int i=0; i<size()-1; i++)
		{
			a[i+1] += a[i]/10;
			a[i] = a[i]%10;
		}
		extend();
	}
	
	void fixNeg()
	{
		for (int i=0; i<size()-1; i++)
		{
			while (a[i] < 0)
			{
				a[i+1] --;
				a[i] += 10;
			}
		}
	}
	
	void dropZero()
	{
		while (a[size()-1] == 0 && size() > 1)
			a.pop_back();
	}
	
	void reverse()
	{
		for (int i=0; i<size()/2; i++)
			swap(a[i], a[size()-i-1]);
	}
	
	BigNum devide2()
	{
		BigNum x = *this, sol;
		for (int i=x.size()-1; i>0; i--)
		{
			sol.a.push_back(x.a[i]/2);
			x.a[i-1] += 10*(x.a[i]%2);
		}
		sol.a.push_back(x.a[0]/2);
		sol.reverse();
		sol.dropZero();
		return sol;
	}
	
	BigNum pos() const
	{
		BigNum x = *this;
		x.sign = 1;
		return x;
	}
	
	BigNum bSearch(BigNum s, BigNum t, BigNum x)
	{
		if (s+1 == t)
			return s;
		BigNum mid = s+t;
		mid = mid.devide2();
		if (*this < mid * x)
			return bSearch(s, mid, x);
		return bSearch(mid, t, x);
	}
	
	bool operator < (const BigNum x)const
	{
		if (sign == x.sign)
		{
			if (size() < x.size())
				return (sign == 1);
			for (int i=size()-1; i>=0; i--)
				if (a[i] != x.a[i])
					return (a[i]<x.a[i])*(sign == 1);
			return false;
		}
		return sign < x.sign;
	}
	bool operator > (const BigNum x)const
	{
		return x < *this;
	}
	
	bool operator == (const BigNum x)const
	{
		return (!(x < *this) && !(*this < x));
	}
	
	BigNum operator + (const BigNum x)const
	{
		BigNum y;
		if (x.sign != sign)
		{
			y = pos() - x.pos();
			y.sign = y.sign*sign;
			return y;
		}
		y = *this;
		y.push(x.size() - size(), 0);
		for (int i=0; i<x.size(); i++)
			y.a[i] += x.a[i];
		y.fixCarry();
		return y;
	}
	BigNum operator * (BigNum x)
	{
		BigNum y;
		y.push(x.size() + size(), 0);
		for (int i=0; i<x.size(); i++)
			for (int j=0; j<size(); j++)
				y.a[i+j] += a[j]*x.a[i];
		y.fixCarry();
		y.dropZero();
		y.sign = sign * x.sign;
		return y;
	}
	
	BigNum operator - (const BigNum x)const 
	{
		BigNum y, b = x;
		if (sign == b.sign)
		{
			if (sign == -1)
				return b.pos() - pos();
			if (b > *this)
			{
				y = b - *this;
				y.sign = -1;
					return y;
			}	
			b.push(size() - b.size(), 0);
			y.push(size(), 0);
			for (int i=0; i<size(); i++)
				y.a[i] = a[i] - b.a[i];
			y.fixNeg();
			y.dropZero();
			return y;
		}
		y = pos() + x.pos();
		y.sign = sign;
		return y;
	}
	
	BigNum operator / (BigNum x)
	{
		if (x == 2)
			return devide2();
		BigNum y;
		y = bSearch(0, *this+1, x);
		y.sign = sign * x.sign;
		return y;
	}
	
	void write()
	{
		if (sign == -1)
			cout << '-';
		for (int i=size()-1; i>=0; i--)
			cout << a[i];
		cout << endl;
	}
};

int main()
{
	BigNum a(234), b(-245), c = a*b;
	c.write();
	return 0;
}