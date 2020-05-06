// Roll.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <map>
#include <windows.h>   
#include "Roll.h"

#define OK 1;
using namespace std;

string _menu = "Menu.txt";
fstream _readFile;
static int _year, _month, _day;
static unsigned int _getKey = 0;
static unsigned int _checkedCount = 0;
static bool _getFlag = false;


#if 1
struct Dinner
{
	char mType = 'D';
	char mTime[8];
	bool isChecked = false;
	int  mWordsLen = 0;
	string mName;
};

map <int, Dinner> mp;

int Read()
{
	Dinner tmpD;
	string tmps;
	int n;
	int key = 0;
	string::iterator it;
	while (getline(_readFile, tmps))
	{
		int n = 0;
		it = tmps.begin();
		//写入类型
		tmpD.mType = (*it);

		//写入时间
		tmps.copy(tmpD.mTime, 4, 2);
		tmps.copy(tmpD.mTime + 4, 2, 6);
		tmps.copy(tmpD.mTime + 6, 2, 8);

		//写入名字
		it += 11;
		while (it != tmps.end())
		{
			n++;
			it++;
		}
		tmpD.mName = tmps.substr(11, n);
		tmpD.mWordsLen = n;

		
		//更新map
		mp.insert(make_pair(key, tmpD));
		key ++;
		/*cout << tmpD.mType << " " << tmpD.mName << " " << tmpD.mTime[0] << tmpD.mTime[1] << tmpD.mTime[2] << tmpD.mTime[3]
			 << tmpD.mTime[4] << tmpD.mTime[5] << tmpD.mTime[6] << tmpD.mTime[7] << endl;*/
		
	}
	return mp.size();
}

string Get()
{
	_getFlag = false;
	if (_getKey >= mp.size())
	{
		_getKey = 0;
		cout << "_getKey reset" << endl;
		//重置后需判断是否所有选项均已判断，避免递归死循环
		if (_checkedCount >= mp.size())
		{
			_getFlag = true;
			return "quit";
		}
		return Get();
	}
	//计算时间差
	int year_t, month_t, day_t;
	year_t = (mp[_getKey].mTime[0] - '0')*1000 + (mp[_getKey].mTime[1] - '0') * 100 + (mp[_getKey].mTime[2] - '0') * 10 + (mp[_getKey].mTime[3] - '0');
	month_t = (mp[_getKey].mTime[4] - '0') * 10 + (mp[_getKey].mTime[5] - '0');
	day_t = (mp[_getKey].mTime[6] - '0') * 10 + (mp[_getKey].mTime[7] - '0');
	int delta_t = (_year - year_t) * 365 + (_month - month_t) * 30 + _day - day_t;
	//cout << delta_t << endl;

	//开始判断
	string Ra = "Roll again ";
	string tmp_s;
	if (mp[_getKey].isChecked)
	{
		_getKey++;
		return Get();
	}
	switch (mp[_getKey].mType)
	{
		case 'D':
			if (delta_t < 4)
			{
				mp[_getKey].isChecked = true;
				_checkedCount++;
				return Ra.append(mp[_getKey].mName);
			}
			else
			{
				if (mp[_getKey].isChecked)
				{
					_getKey++;
					return Get();
				}
				else
				{
					mp[_getKey].isChecked = true;
					_checkedCount++;
					_getFlag = true;
					return mp[_getKey].mName;
				}
			}
			break;
		case 'M':
			if (delta_t < 30)
			{
				mp[_getKey].isChecked = true;
				_checkedCount++;
				return Ra.append(mp[_getKey].mName);
			}
			else
			{
				if (mp[_getKey].isChecked)
				{
					_getKey++;
					return Get();
				}
				else
				{
					mp[_getKey].isChecked = true;
					_checkedCount++;
					_getFlag = true;
					return mp[_getKey].mName;
				}
			}
			break;
		case 'Y':
			if (delta_t < 365)
			{
				mp[_getKey].isChecked = true;
				_checkedCount++;
				return Ra.append(mp[_getKey].mName);
			}
			else
			{
				if (mp[_getKey].isChecked)
				{
					_getKey++;
					return Get();
				}
				else
				{
					mp[_getKey].isChecked = true;
					_checkedCount++;
					_getFlag = true;
					return mp[_getKey].mName;
				}
			}
			break;
		default:
			_getFlag = true;
			return mp[_getKey].mName;
	}
}

void UpdateTime()
{
	int i,offset = 0;
	//制式化日期格式
	char mon_t[2];
	char day_t[2];
	mon_t[0] = _month / 10 + 48;
	mon_t[1] = _month % 10 + 48;
	day_t[0] = _day / 10 + 48;
	day_t[1] = _day % 10 + 48;

	if(_getKey != 0)
	{
		for (i = 0; i < _getKey; i++)
		{
			offset += mp[i].mWordsLen;
		}
		offset = offset + 12 * i;
		//write time
	}
	_readFile.seekg(offset);
	_readFile << mp[_getKey].mType << " " << _year << mon_t[0] << mon_t[1] << day_t[0] << day_t[1];
}
#endif

int main()
{	

	//获取系统时间
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	_year = sys.wYear;
	_month = sys.wMonth;
	_day = sys.wDay; 

	//打开文件
	_readFile.open(_menu, ios::in | ios::out);
	if (_readFile.fail())
	{
		cout << "error: open _readFile failed" << endl;
		return 0;
	}
	//读入数据到map
	int count = Read();
	if (!count)
	{
		cout << "error: unavailable menu, please check!" << endl;
		return 0;
	}
	//获取结果
	string _result;
	while (!_getFlag)
	{
		//获取到随机值
		srand((unsigned)time(NULL));
		_getKey = rand() % (count - 1);
		//获取结果
		string _result = Get();	
		cout << _result << endl;
		if (!_result.compare("quit"))
		{
			cout << "there is no available choice" << endl;
			return 0;
		}
		//判断是否想吃
		char isWanttoEat;
		if (_getFlag) 
		{
			cin >> isWanttoEat;
			if (isWanttoEat == 'N' || isWanttoEat == 'n')
			{
				_getFlag = false;
			}
			else
			{
				//更新时间
				_readFile.clear();
				UpdateTime();
			}
		}
		else 
		{
			Sleep(800);
		}
	}
	_readFile.close();
	return 0;
}

