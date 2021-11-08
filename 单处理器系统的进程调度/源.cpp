#include <iostream>
#include <queue>
#include <vector>
#define MAX_PCB_COUNT 10
#define OK 1
#define FAILED -1
#define TIMESLICE 2//时间片大小

using namespace std;

typedef struct PCB
{
	int ax, bx, cx, dx;//通用寄存器
	int pc;//程序寄存器
	int psw;//程序状态字寄存器

	int PID;//进程标识符
	int timeRequire;//进程申请运行的时间

	bool operator ==(PCB pcb)//定义PID相同，即为同一进程
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
queue <PCB> idle;//空闲的PCB队列
vector <PCB> wait;//等待的PCB
queue <PCB> ready;//就绪的PCB队列

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

int Pcreate(int PID, int timeRequire)//创建进程，即申请PCB
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
	while (!ready.empty())//运行队列中的进程，并保存Log
	{
		PCB nowProcess = ready.front();
		if (nowProcess.timeRequire > TIMESLICE)//进程一次执行不完时
		{
			nowProcess.timeRequire -= TIMESLICE;
			ready.pop();
			ready.push(nowProcess);
			runLog log(nowProcess.PID, TIMESLICE);
			myRunLog.push_back(log);
		}
		else //当进程一次可以执行完时
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