#include <iostream>
#include <ctime>
#include <Windows.h>
#include <random>
#include <map>

using namespace std;
int main()
{
	default_random_engine e(time(0));
	uniform_int_distribution<unsigned int> u(1, 6);
	unsigned int Players, Rounds;
	unsigned int maxp, minp;
	unsigned int temp_roll;  //临时储存点数
	unsigned  char Rule; //用于选择使用规则
	cout << "请输入游戏人数：\n";
	cin >> Players;
	cout << "请输入局数：\n";
	cin >> Rounds;
	cout << "请选择当前使用规则\n\"0\"为普通规则\n\"1\"为通用规则\n\"2\"为鬼畜规则\n";
	cin >> Rule;
	/*//测试数据
	Players = 8;
	Rounds = 1000000;
	Rule = '2';
	*/
	map<unsigned int,unsigned int> result_win;  //储存获胜结果的字典
	map<unsigned int,unsigned int> result_lose;  //储存失败结果的字典
	for (int d = 1; d <= 6; d++)  //对字典进行初始化
	{
		result_win.insert(pair<unsigned int,unsigned int>(d, 0));
		result_lose.insert(pair<unsigned int,unsigned int>(d, 0));
	}
	switch (Rule)
	{
	case '0':
	{
		for (unsigned int round = 1; round <= Rounds; round++)
		{
			maxp = 0;  //对最大值和最小值进行初始化
			minp = 20;  //
			for (unsigned int player = 1; player <= Players; player++)
			{
				temp_roll = u(e);
				if (temp_roll > maxp) maxp = temp_roll;
				if (temp_roll < minp) minp = temp_roll;
			}
			if (maxp != minp)
			{
				result_win[maxp] += 1;
				result_lose[minp] += 1;
			}
		}
	}break;
	case '1':
	{
		map<unsigned int, unsigned int> single_round;
		for (int d = 1; d <= 6; d++)  //对字典进行初始化
		{
			single_round.insert(pair<unsigned int, unsigned int>(d, 0));
		}
		for (unsigned  round = 1; round <= Rounds; round++)
		{
			maxp = 0;  //对最大值和最小值进行初始化
			minp = 20;  //
			bool four = 0;
			for (int d = 1; d <= 6; d++)  //对字典进行初始化
			{
				single_round[d] = 0;
			}
			for (unsigned int player = 1; player <= Players; player++)
			{
				temp_roll = u(e);
				single_round[temp_roll] += 1;  //储存点数
				if (temp_roll > maxp) maxp = temp_roll;
				if (temp_roll < minp) minp = temp_roll;
			}
			if (maxp == minp)continue;
			if (single_round[4] > single_round[6] && single_round[6] != 0)  //如果场上4比6多，4变为7，即临时4获胜
			{
				maxp = 4;
				if (minp == 4 && single_round[5] == 0) minp = 6;
				else minp = 5;
			}
			if (single_round[1] > single_round[maxp]) //如果场上1比最大点多，1变为8，即1获胜
			{
				maxp = 1;
				if (four == 1)
				{
					if (single_round[3] == 0) minp = 4;
				}
				while (single_round[minp + 1] == 0)
				{
					minp += 1;  //对最小点进行移动
				}
				minp += 1;
			}
			result_win[maxp] += 1;
			result_lose[minp] += 1;
		}
		single_round.clear();
	}break;
	case '2':
	{
		map<unsigned int, unsigned int> single_round;
		for (int d = 1; d <= 6; d++)  //对字典进行初始化
		{
			single_round.insert(pair<unsigned int, unsigned int>(d, 0));
		}
		for (unsigned int round = 1; round <= Rounds; round++)
		{
			maxp = 0;  //对最大值和最小值进行初始化
			minp = 20;  //
			for (int d = 1; d <= 6; d++)  //对字典进行初始化
			{
				single_round[d] = 0;
			}
			bool one = 0, two = 0, four = 0, five = 0; //旗帜初始化
			for (unsigned int player = 1; player <= Players; player++)
			{
				temp_roll = u(e);
				single_round[temp_roll] += 1;  //储存点数
				if (temp_roll > maxp) maxp = temp_roll;
				if (temp_roll < minp) minp = temp_roll;
			}
			if (maxp == minp)continue;
			if (single_round[4] > single_round[6] && single_round[6] != 0)  //如果场上4比6多，4变为7，即临时4获胜
			{
				four = 1;  //进行四判定，旗帜置1
				if (single_round[4] >= single_round[5])  maxp = 4;
				else single_round[2] += single_round[4];
				if (minp == 4 && single_round[5] == 0) minp = 6;
				else minp = 5;
			}
			if (single_round[1] > single_round[maxp]) //如果场上1比最大点多，1变为8，即1获胜
			{
				one = 1;
				maxp = 1;
				if (four == 1)
				{
					if (single_round[3] == 0 && single_round[2] == 0) minp = 4;
				}
				while (single_round[++minp] == 0) {}
			}
			if (single_round[2] + 1 < single_round[maxp]) //二点刺杀
			{
				two = 1;
				maxp = 2;
				if (minp == 2)
				{
					if (four == 1)
					{
						if (single_round[3] == 0)minp = 4;
					}
					while (single_round[++minp] == 0) 
					{
						if (minp == 6 )  //如果正常范围内无法找到最小值，考虑变换后的点数
						{
							if(four)
							{
								minp = 4;
								break;
							}
							else if (one)
							{
								minp = 1;
								break;
							}
						}
					}
				}
			}
			if (maxp == 6 && single_round[6] >= (Players / 2 + Players % 2))  //六点自爆
			{
				minp = 6;
				while (single_round[--maxp] == 0) {}
			}
			if (maxp == 6 && (single_round[5] + 1) < single_round[6] && single_round[5] != 0)  //五点权贵
			{
				five = 1;
				if (minp == 5)continue;
			}
			result_win[maxp] += 1;
			result_lose[minp] += 1;
			if (maxp == 2 && four == 1)result_win[4] += 1;
			if (maxp == 6 && five == 1)result_win[5] += 1;
		}
		single_round.clear();
	}break;
	default:
	{
		cout << "由于您选择的规则有误，本机开始自爆倒计时\n";
		for (int i = 10; i >= 0; i--)
		{
			cout << i << endl;
			Sleep(1000);
		}
		return 0;
	}
	}
	cout << "\n\n获胜情况：\n";
	for (unsigned int i = 1; i <= 6; i++)
	{
		cout << i << "：\t" << double(result_win[i]) * 100 / double(Rounds) << "%\n";
	}
	cout << "\n";
	cout << "\n\n落败情况：\n";
	for (unsigned int i = 1; i <= 6; i++)
	{
		cout << i << "：\t" << double(result_lose[i]) * 100  / double(Rounds)<< "%\n";
	}
	cout << "\n";
	system("pause");
	return 0;
}