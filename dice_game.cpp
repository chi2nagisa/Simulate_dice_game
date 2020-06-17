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
	unsigned int temp_roll;  //��ʱ�������
	unsigned  char Rule; //����ѡ��ʹ�ù���
	cout << "��������Ϸ������\n";
	cin >> Players;
	cout << "�����������\n";
	cin >> Rounds;
	cout << "��ѡ��ǰʹ�ù���\n\"0\"Ϊ��ͨ����\n\"1\"Ϊͨ�ù���\n\"2\"Ϊ�������\n";
	cin >> Rule;
	/*//��������
	Players = 8;
	Rounds = 1000000;
	Rule = '2';
	*/
	map<unsigned int,unsigned int> result_win;  //�����ʤ������ֵ�
	map<unsigned int,unsigned int> result_lose;  //����ʧ�ܽ�����ֵ�
	for (int d = 1; d <= 6; d++)  //���ֵ���г�ʼ��
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
			maxp = 0;  //�����ֵ����Сֵ���г�ʼ��
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
		for (int d = 1; d <= 6; d++)  //���ֵ���г�ʼ��
		{
			single_round.insert(pair<unsigned int, unsigned int>(d, 0));
		}
		for (unsigned  round = 1; round <= Rounds; round++)
		{
			maxp = 0;  //�����ֵ����Сֵ���г�ʼ��
			minp = 20;  //
			bool four = 0;
			for (int d = 1; d <= 6; d++)  //���ֵ���г�ʼ��
			{
				single_round[d] = 0;
			}
			for (unsigned int player = 1; player <= Players; player++)
			{
				temp_roll = u(e);
				single_round[temp_roll] += 1;  //�������
				if (temp_roll > maxp) maxp = temp_roll;
				if (temp_roll < minp) minp = temp_roll;
			}
			if (maxp == minp)continue;
			if (single_round[4] > single_round[6] && single_round[6] != 0)  //�������4��6�࣬4��Ϊ7������ʱ4��ʤ
			{
				maxp = 4;
				if (minp == 4 && single_round[5] == 0) minp = 6;
				else minp = 5;
			}
			if (single_round[1] > single_round[maxp]) //�������1������࣬1��Ϊ8����1��ʤ
			{
				maxp = 1;
				if (four == 1)
				{
					if (single_round[3] == 0) minp = 4;
				}
				while (single_round[minp + 1] == 0)
				{
					minp += 1;  //����С������ƶ�
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
		for (int d = 1; d <= 6; d++)  //���ֵ���г�ʼ��
		{
			single_round.insert(pair<unsigned int, unsigned int>(d, 0));
		}
		for (unsigned int round = 1; round <= Rounds; round++)
		{
			maxp = 0;  //�����ֵ����Сֵ���г�ʼ��
			minp = 20;  //
			for (int d = 1; d <= 6; d++)  //���ֵ���г�ʼ��
			{
				single_round[d] = 0;
			}
			bool one = 0, two = 0, four = 0, five = 0; //���ĳ�ʼ��
			for (unsigned int player = 1; player <= Players; player++)
			{
				temp_roll = u(e);
				single_round[temp_roll] += 1;  //�������
				if (temp_roll > maxp) maxp = temp_roll;
				if (temp_roll < minp) minp = temp_roll;
			}
			if (maxp == minp)continue;
			if (single_round[4] > single_round[6] && single_round[6] != 0)  //�������4��6�࣬4��Ϊ7������ʱ4��ʤ
			{
				four = 1;  //�������ж���������1
				if (single_round[4] >= single_round[5])  maxp = 4;
				else single_round[2] += single_round[4];
				if (minp == 4 && single_round[5] == 0) minp = 6;
				else minp = 5;
			}
			if (single_round[1] > single_round[maxp]) //�������1������࣬1��Ϊ8����1��ʤ
			{
				one = 1;
				maxp = 1;
				if (four == 1)
				{
					if (single_round[3] == 0 && single_round[2] == 0) minp = 4;
				}
				while (single_round[++minp] == 0) {}
			}
			if (single_round[2] + 1 < single_round[maxp]) //�����ɱ
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
						if (minp == 6 )  //���������Χ���޷��ҵ���Сֵ�����Ǳ任��ĵ���
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
			if (maxp == 6 && single_round[6] >= (Players / 2 + Players % 2))  //�����Ա�
			{
				minp = 6;
				while (single_round[--maxp] == 0) {}
			}
			if (maxp == 6 && (single_round[5] + 1) < single_round[6] && single_round[5] != 0)  //���Ȩ��
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
		cout << "������ѡ��Ĺ������󣬱�����ʼ�Ա�����ʱ\n";
		for (int i = 10; i >= 0; i--)
		{
			cout << i << endl;
			Sleep(1000);
		}
		return 0;
	}
	}
	cout << "\n\n��ʤ�����\n";
	for (unsigned int i = 1; i <= 6; i++)
	{
		cout << i << "��\t" << double(result_win[i]) * 100 / double(Rounds) << "%\n";
	}
	cout << "\n";
	cout << "\n\n��������\n";
	for (unsigned int i = 1; i <= 6; i++)
	{
		cout << i << "��\t" << double(result_lose[i]) * 100  / double(Rounds)<< "%\n";
	}
	cout << "\n";
	system("pause");
	return 0;
}