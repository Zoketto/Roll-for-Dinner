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
using namespace std;

string menu = "Menu.txt";
static int _year, _month, _day;


#if 1
struct Dinner
{
	
	char _type;
	string _name;
	char time[8];
};

map <int, Dinner> mp;

int Read(string menu, ifstream& file)
{
	Dinner tmpd;
	string tmps;
	int n;
	int key = 0;
	string::iterator it;
	while (getline(file, tmps))
	{
		n = 0;
		it = tmps.begin();
		//写入类型
		tmpd._type = (*it);
		it += 3;
		while ((*it) != ' ' && it != tmps.end())
		{
			n ++;
			it++;;
		}
		//写入名字
		tmpd._name = tmps.substr(3, n);
		//写入时间
		tmps.copy(tmpd.time, 4, n += 4);
		tmps.copy(tmpd.time+4, 2, n += 4 );
		tmps.copy(tmpd.time+6, 2, n += 2);
		mp.insert(make_pair(key, tmpd));
		key ++;
		cout << tmpd._type << " " << tmpd._name << " " << tmpd.time[0] << tmpd.time[1] << tmpd.time[2] << tmpd.time[3]
			 << tmpd.time[4] << tmpd.time[5] << tmpd.time[6] << tmpd.time[7] << endl;
		
	}
	return mp.size();
}

string Get(int rand, bool &flag, ifstream& file)
{
	//cout << rand << endl;
	if (rand >= mp.size())
	{
		return "rand is error";
	}
	//计算差值
	int year_t, month_t, day_t;
	year_t = (mp[rand].time[0] - '0')*1000 + (mp[rand].time[1] - '0') * 100 + (mp[rand].time[2] - '0') * 10 + (mp[rand].time[3] - '0');
	month_t = (mp[rand].time[4] - '0') * 10 + (mp[rand].time[5] - '0');
	day_t = (mp[rand].time[6] - '0') * 10 + (mp[rand].time[7] - '0');
	int delta = (_year - year_t) * 365 + (_month - month_t) * 30 + _day - day_t;
	cout << delta << endl;
	string Ra = "Roll again ";
	string tmp_s;
	switch (mp[rand]._type)
	{
		case 'D':
			if (delta < 4)
			{
				return Ra.append(mp[rand]._name);
			}
			else
			{
				//修改时间
				int tmp_l = 0;
				file.clear();
				file.seekg(0);
				while (getline(file, tmp_s) && tmp_l < 1)
				{
					cout << "zz" << tmp_s << endl;
				}
				flag = true;
				return mp[rand]._name;
			}
			break;
		case 'M':
			if (delta < 30)
			{
				return Ra.append(mp[rand]._name);
			}
			else
			{
				//修改时间
				int tmp_l = 0;
				file.clear();
				file.seekg(0);
				while (getline(file, tmp_s) && tmp_l < 1)
				{
					cout << tmp_s << endl;
				}
				flag = true;
				return mp[rand]._name;
			}
			break;
		case 'Q':
			if (delta < 365)
			{
				return Ra.append(mp[rand]._name);
			}
			else
			{
				//修改时间
				int tmp_l = 0;
				file.clear();
				file.seekg(0);
				while (getline(file, tmp_s) && tmp_l < 1)
				{
					cout << tmp_s << endl;
				}
				flag = true;
				return mp[rand]._name;
			}
			break;
		default:
			return mp[rand]._name;
	}
}
#endif

int main()
{	
	ifstream ReadFile;
	//获取时间
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	_year = sys.wYear;
	_month = sys.wMonth;
	_day = sys.wDay; 
	//打开文件
	ReadFile.open(menu, ios::in | ios::out);
	//读入数据到map
	if (ReadFile.fail())
	{
		cout << "error: open file failed" << endl;
		return 0;
	}

	int count = Read(menu, ReadFile);
	
	//获取结果
	string _result;
	bool getted = false;
	int get_s;
	while (!getted)
	{
		//获取到随机值
		srand((unsigned)time(NULL));
		get_s = rand() % (count - 1);
		_result = Get(get_s, getted, ReadFile);	//获取结果
		getted = true;
		cout << _result << endl;
	}
	return 0;
}

