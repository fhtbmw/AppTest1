#include <vector>
#include <map>
#include <iostream>
#include <algorithm>

using namespace std;

template<typename ... Args>
class Solution
{
public:

	virtual void merge(Args... a) {}
	virtual int removeElement(Args... a) { return 0; }
	virtual bool isPalindrome(Args... a) { return false; }
	virtual bool canConstruct(Args ... a) { return false; }
	virtual int removeElement2(Args ... a) { return 0; }
	virtual bool isSubsequence(Args ... a) { return false; }
	virtual int removeDuplicates(Args... a) { return 0; }
	virtual vector<int> twoSum(vector<int>& numbers, int target) { return NULL; }
};

template<typename ... Args>
class SubofS : public Solution<Args ... >
{
public:
};

//合并有序数组
template<typename ... Args>
class Cmerge : public Solution<Args ...>
{
public:

	void merge(vector<int>& num1, int m, vector<int>& num2, int n)override
	{
		vector<int>vt1;
		vt1.resize(m + n);

		int index1 = 0;
		int index2 = 0;

		for (int i = 0; i < m + n; i++)
		{
			/*vt1[i] = (num1[index1] <= num2[index2]) ? num1[index1] : num2[index2];*/
			if (index1 < m && index2 < n)
			{
				if (num1[index1] <= num2[index2])
				{
					vt1[i] = num1[index1++];
				}
				else
				{
					vt1[i] = num2[index2++];
				}
			}
			else if (index1 < m)
			{
				vt1[i] = num1[index1++];
			}
			else
			{
				vt1[i] = num2[index2++];
			}
		}


		for (auto e : vt1)
		{
			cout << e << endl;
		}

	}

};

//移除指定值
template<typename ...Args>
class Cremoveelement : public Solution<Args ...>
{
public:
	int removeElement(vector<int>& vtnum, int target)
	{
		/*std::sort(vtnum.begin(), vtnum.end());
		int l = 0;
		int r = vtnum.size();
		int index = 0;
		int count = 0;
		while (l<r)
		{
			index = (l + r) / 2;
			if (vtnum[index] > target)
			{
				r = index;
			}
			else if(vtnum[index] < target)
			{
				l = index;
			}
			else
			{
				count++;
			}
		}*/

		/*int l = 0;
		int r = vtnum.size() - 1;
		while (l < r)
		{
			if (vtnum[l] != target)
			{
				l++;
			}
			else if (vtnum[r] == target)
			{
				r--;
			}
			else
			{
				vtnum[l] = vtnum[r];
				r--;
			}

		}*/

		vtnum.erase((std::remove_if(vtnum.begin(), vtnum.end(), [=](int a) { return a == target; })), vtnum.end());

		for (auto e : vtnum)
		{
			cout << e << endl;
		}

		return 1;
	}
};

template<typename ... Args>
class Cispalindrome : public Solution<Args ...>
{
public:
	//回文字符串
	static bool isNumOrPha(char cin)
	{
		return ((0 < cin && cin < 9) || ('a' < cin && cin < 'z'));
	}

	bool isPalindrome(string _s) override
	{
		if (_s.empty())
		{
			return true;
		}

		int l = 0, r = _s.size() - 1;

		_s.erase(std::remove_if(_s.begin(), _s.end(), [](char c) { return !isNumOrPha(c); }), _s.end()); //remove_if将满足条件的元素放入容器末尾，返回新范围的末尾迭代器

		std::string temp = _s;
		std::reverse(temp.begin(), temp.end());

		return temp == _s;
	}
};

template<typename ...Args>
class Ccanconstruct : public Solution<Args...>
{
	//383 
	//给你两个字符串：ransomNote 和 magazine ，判断 ransomNote 能不能由 magazine 里面的字符构成。
	bool canConstruct(string ransomNote, string magazine) override {
		for (int i = 0; i < magazine.size(); i++)
		{
			if (magazine.find(ransomNote[i]) != string::npos)
			{
				continue;
			}
			else
			{
				return false;
			}
		}
		return true;
	}
};

template<typename ...Args>
class CremoveElement2 : public Solution<Args ...>
{
public:
	//给你一个数组 nums 和一个值 val，
	// 你需要 原地 移除所有数值等于 val 的元素。元素的顺序可能发生改变。然后返回 nums 中与 val 不同的元素的数量。
	int removeElement2(vector<int>& nums, int val) override {
		nums.erase(std::remove_if(nums.begin(), nums.end(), [=](int n) {return n == val; }), nums.end());
		return nums.size();
	}
};

template<typename ...Args>
class Cissubsquence : public Solution<Args ...>
{
public:

	bool isSubsequence(string s, string t) override
	{
		int i = 0, j = 0;
		while (i < s.length() && j < t.length())
		{
			if (s[i] == t[j])
			{
				i++;
			}
			j++;
		}
		return i == s.length();
	}

	//Dp: 自底向上
	bool isSubsequence2(string s, string t)
	{
		
	}
};

template<typename ... Args>
class Cissubsequence : public Solution<Args ...>
{
public:
	int removeDuplicates(vector<int>& nums) override
	{
		std::map<int, int> _map;

		vector<int>vttemp = nums;

		for (auto n : nums)
		{
			if (_map.find(n) != _map.end())
			{
				_map[n]++;
				continue;
			}
			_map[n] = 1;
		}

		nums.erase(std::remove_if(nums.begin(), nums.end(), [=](int in) { return (_map.at(in)> 1); }), nums.end());

		for (auto e : nums)
		{
			cout << e << endl;
		}
		return nums.size();
	}

};

template<typename ... Args>
class CtwoSum : public Solution<Args ...>
{
public:
	//NO. 167 一个下标从 1 开始的整数数组 numbers ，该数组已按 非递减顺序排列，
	//从数组中找出满足相加之和等于目标数 target 的两个数。
	vector<int> twoSum(vector<int>& numbers, int target) override {


	}


};

void main()
{
	//Solution<vector<int>&, int, vector<int>&, int>* s1 = new Cmerge<vector<int>&, int, vector<int>&, int>();
	//std::vector<int>vt1{ 2,3,4 };
	//std::vector<int>vt2{ 23,3,4 };
	//s1->merge(vt1, 2, vt2, 3 );
	//s1->TempFunc();

	/*Solution<vector<int>&, int>* s2 = new Cremoveelement<vector<int>&, int>();
	std::vector<int>vt3{ 2,3,4,45 };
	int num = 2;
	s2->removeElement(vt3, 2);*/

	//Solution<string>* s3 = new Cispalindrome<string>();
	//s3->isPalindrome("dasd,dwd");

	/*Solution<string, string>* s4 = new Ccanconstruct<string, string>();
	s4->canConstruct("dB", "Alasdair's");*/

	//Solution<vector<int>&, int>* s5 = new CremoveElement2<vector<int>&, int>();
	//std::vector<int> vt1{ 3, 2, 2, 3 };
	//cout << s5->removeElement2(vt1, 3);

	/*Solution<string, string>* s6 = new Cissubsquence<string, string>();
	cout << s6->isSubsequence("dds", "ddssss");*/

	Solution<vector<int>&>* s7 = new Cissubsequence<vector<int>&>();
	std::vector<int> vt3{ 2,2,3,3,41,1 };
	s7->removeDuplicates(vt3);
  }


