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
		}
	}
	return myRunLog;
}

int testmain()
{
	init();
	Pcreate(8, 5);
	Pcreate(3, 3);
	Pready(8);
	Pready(3);
	Prun();
	return OK;
}

int main()
{
	testmain();
}