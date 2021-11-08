#include <iostream>
#include <queue>
#include <vector>
#define MAX_PCB_COUNT 10
#define OK 1
#define FAILED -1
#define TIMESLICE 2//ʱ��Ƭ��С

using namespace std;

typedef struct PCB
{
	int ax, bx, cx, dx;//ͨ�üĴ���
	int pc;//����Ĵ���
	int psw;//����״̬�ּĴ���

	int PID;//���̱�ʶ��
	int timeRequire;//�����������е�ʱ��

	bool operator ==(PCB pcb)//����PID��ͬ����Ϊͬһ����
	{
		return (pcb.PID == PID);
	}
};

int counter = 0;

typedef struct runLog
{
	int PID;
	int runTime;

	runLog(int myPID, int myrunTime) :PID(myPID), runTime(myrunTime) {};
};
queue <PCB> idle;//���е�PCB����
vector <PCB> wait;//�ȴ���PCB
queue <PCB> ready;//������PCB����

int init()
{
	for (int i = 0; i < MAX_PCB_COUNT; i++)
	{
		PCB pcb;
		pcb.pc = i;
		idle.push(pcb);
	}

	return OK;
}

int Pcreate(int PID, int timeRequire)//�������̣�������PCB
{
	if (idle.empty())
	{
		return FAILED;
	}

	PCB toAllocate = idle.front();
	toAllocate.PID = PID;
	toAllocate.timeRequire = timeRequire;
	wait.push_back(toAllocate);
	idle.pop();
	return OK;
}

int Pready(int PID)
{
	PCB temp;
	temp.PID = PID;
	vector<PCB>::iterator toReady;
	toReady = find(wait.begin(), wait.end(), temp);
	if (toReady == wait.end())
	{
		return FAILED;
	}

	ready.push(*toReady);
	wait.erase(toReady);
	return OK;
}

vector<runLog> Prun()
{
	vector<runLog> myRunLog;
	while (!ready.empty())//���ж����еĽ��̣�������Log
	{
		PCB nowProcess = ready.front();
		if (nowProcess.timeRequire > TIMESLICE)//����һ��ִ�в���ʱ
		{
			nowProcess.timeRequire -= TIMESLICE;
			ready.pop();
			ready.push(nowProcess);
			runLog log(nowProcess.PID, TIMESLICE);
			myRunLog.push_back(log);
		}
		else //������һ�ο���ִ����ʱ
		{
			ready.pop();
			runLog log(nowProcess.PID, nowProcess.timeRequire);
			myRunLog.push_back(log);
			idle.push(nowProcess);
		}
	}
	return myRunLog;
}

int explore()
{
	cout << "��ǰ����PCB������" << idle.size() << endl;
	cout << "===========================================" << endl;
	cout << "��ǰ�ȴ��Ľ���" << endl;
	cout << "PID\t����ʱ��\t" << endl;
	vector<PCB>::iterator iter;
	for (iter = wait.begin(); iter != wait.end(); iter++)
	{
		cout << iter->PID << "\t" << iter->timeRequire << endl;
	}
	cout << "===========================================" << endl;
	cout << "��ǰ�����Ľ���" << endl;
	cout << "PID\t����ʱ��\t" << endl;
	for (int i = 0; i < ready.size(); i++)
	{
		PCB process = ready.front();
		cout << process.PID << "\t" << process.timeRequire << endl;
		ready.pop();
		ready.push(process);
	}
	cout << "===========================================" << endl << endl;
	return OK;
}

int runResult(vector<runLog> log)
{
	cout << "����������־: " << endl;
	cout << "===========================================" << endl;
	cout << "ʱ��Ƭ���\tPID\tִ��ʱ��" << endl;
	vector<runLog>::iterator iter;
	for (iter = log.begin(); iter != log.end(); iter++)
	{
		cout << iter - log.begin() + 1 << "\t\t" << iter->PID << "\t" << iter->runTime << endl;
	}

	cout << "===========================================" << endl << endl;

	return OK;
}

int testmain()
{
	init();
	Pcreate(8, 5);
	Pcreate(3, 3);
	explore();
	Pready(8);
	Pready(3);
	explore();
	vector<runLog> res = Prun();
	runResult(res);
	explore();
	return OK;
}

int menu()
{
	cout << "===========================================" << endl;
	cout << "���̵���ģ�����" << endl;
	cout << "��ѡ��Ҫִ�еĲ�����" << endl;
	cout << "1. ����һ������" << endl;
	cout << "2. ʹ���̽������̬" << endl;
	cout << "3. ���о����Ľ���" << endl;
	cout << "4. ��ʾ���н���״̬" << endl;
	cout << "===========================================" << endl << endl;
	int option;
	cin >> option;
	return option;
}

int main()
{
	init();
	vector<runLog> rlog;
	int PID;
	while (1)
	{
		switch (menu())
		{
		case 1:
			cout << "Ҫ�����Ľ��̺�: ";
			cin >> PID;
			cout << "�������������ʱ��: ";
			int timeRequire;
			cin >> timeRequire;
			if (Pcreate(PID, timeRequire) == FAILED)
			{
				cout << "���̴���ʧ�ܣ�" << endl;
			}
			else
			{
				cout << "�����ɹ���" << endl;
			}
			break;
		case 2:
			cout << "Ҫ�����Ľ��̺�: ";
			cin >> PID;
			if (Pready(PID) == FAILED)
			{
				cout << "���̾���ʧ�ܣ�" << endl;
			}
			else
			{
				cout << "�����Ѿ���" << endl;
			}
			break;
		case 3:
			rlog = Prun();
			runResult(rlog);
			break;
		case 4:
			explore();
			break;
		default:
			cout << "��������" << endl;
			break;
		}
	}

}