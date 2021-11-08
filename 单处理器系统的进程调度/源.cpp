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

int counter = 0;

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
			idle.push(nowProcess);
		}
	}
	return myRunLog;
}

int explore()
{
	cout << "当前空闲PCB数量：" << idle.size() << endl;
	cout << "===========================================" << endl;
	cout << "当前等待的进程" << endl;
	cout << "PID\t请求时间\t" << endl;
	vector<PCB>::iterator iter;
	for (iter = wait.begin(); iter != wait.end(); iter++)
	{
		cout << iter->PID << "\t" << iter->timeRequire << endl;
	}
	cout << "===========================================" << endl;
	cout << "当前就绪的进程" << endl;
	cout << "PID\t请求时间\t" << endl;
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
	cout << "进程运行日志: " << endl;
	cout << "===========================================" << endl;
	cout << "时间片序号\tPID\t执行时间" << endl;
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
	cout << "进程调度模拟程序" << endl;
	cout << "请选择要执行的操作：" << endl;
	cout << "1. 创建一个进程" << endl;
	cout << "2. 使进程进入就绪态" << endl;
	cout << "3. 运行就绪的进程" << endl;
	cout << "4. 显示所有进程状态" << endl;
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
			cout << "要创建的进程号: ";
			cin >> PID;
			cout << "进程申请的运行时间: ";
			int timeRequire;
			cin >> timeRequire;
			if (Pcreate(PID, timeRequire) == FAILED)
			{
				cout << "进程创建失败！" << endl;
			}
			else
			{
				cout << "创建成功！" << endl;
			}
			break;
		case 2:
			cout << "要就绪的进程号: ";
			cin >> PID;
			if (Pready(PID) == FAILED)
			{
				cout << "进程就绪失败！" << endl;
			}
			else
			{
				cout << "进程已就绪" << endl;
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
			cout << "输入有误" << endl;
			break;
		}
	}

}