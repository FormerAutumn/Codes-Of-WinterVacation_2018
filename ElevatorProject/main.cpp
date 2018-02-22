#include "elevator.h"
#include <bits/stdc++.h>
#define N 5
#define TOP 10
using namespace std;

//1 == up, 0 == down

class Elevator  Lift;
class Passenger Man[N + 5];
queue<int> pick;	//pick up
stack<int> send;	//send to
int cntprint = 0;	//count the amount of arrived passengers

bool cmp( Passenger a, Passenger b )
{
	return a.asktime < b.asktime;
}

//on the way
int judge(int i)
{
	//up
	if ( Lift.nowdir )
	{
		if ( Man[i].fromfloor >= Lift.nowfloor /*&& Man[i].dir == Lift.nowdir*/ )
			return 1;
	}
	//down
	else
	{
		if ( Man[i].fromfloor <= Lift.nowfloor /*&& Man[i].dir == Lift.nowdir*/ )
			return 1;
	}
	
	return 0;
}

void sum_print()
{
	cout << endl;
	
	int maxasktime = -1;
	for (int i = 1; i <= N; i ++)
	{
		if ( Man[i].asktime > maxasktime ) maxasktime = Man[i].asktime;
		Lift.sumtime += Man[i].waitingtime;
	}

	if ( Lift.sumtime < 0 ) Lift.sumtime = maxasktime + Lift.sumtime;
	cout << "The total wating time: " << Lift.sumtime << " s." <<endl;
}

//find out the on the way men
void find(int& nowtime, int nowfloor, int tmptime)
{	
	int cntpick = 0;	//cnt pick people on the way
	for (int i = 1; i <= N; i ++)
	{
		int val = nowtime + fabs( Man[i].fromfloor - nowfloor );
//		cout << "find val " << val << " " << nowtime << " " << tmptime << " " << Man[i].fromfloor << endl;
//		cout << Man[i].vis << tmptime << Man[i].asktime << val << judge( i ) << endl;
		if ( !Man[i].vis && tmptime >= Man[i].asktime
							//not on lift and ask before arrived tmptime
						 && val 	>= Man[i].asktime
						 	//asktime <= the time whem lift pass the callfloor
						 && judge( i ) )
						 	//on the way
		{	
//			cout << "reach find if" << endl;
			pick.push( i );
			Man[i].vis = 1;
			//cntpick ++;
		}
	}
	//nowtime += cntpick;	
//	cout << "find cntpick" << cntpick << " " << nowtime << endl;
}


//work out the asktime is enough or not to cover the movetime
int add(int i, int nowtime, int nowfloor)
{
	int nowval = fabs( nowfloor - Man[i].fromfloor );
	int nowadd = 0;
	if ( Man[i].asktime >= nowtime + nowval )
		nowadd = Man[i].asktime;
	else nowadd = nowval;
//	cout << "NOWUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUADD:" << nowadd << endl;
	return nowadd;
}

//whether or not some can arrive on the way
int arrived(int nowfloor,int& nowtime)
{
	int cnt = 0;
	int cost[TOP + 5];	//cost of off
	memset( cost, 0, sizeof(cost) );
	for (int i = 1; i <= N; i ++)
	{
		if ( !Man[i].off && Man[i].tofloor == nowfloor )
		{
			Man[i].off = 1;
			cost[ Man[i].tofloor ] = 1;
			cnt ++;
//			cout << "in pick arrive" << endl;
			Lift.Print();
			Man[i].Print();
			Man[i].waitingtime = nowtime - Man[i].asktime;
			nowtime ++;	//cost of off
			Lift.nowtime = nowtime;
		}
	}
	return cnt;
}

int reach(int& nowtime, int nowfloor, int tofloor)
{
	int cost[TOP + 5];	//cost of off
	memset( cost, 0, sizeof(cost) );
	int btw(int i, int nowfloor, int tofloor);
	int cnt = 0;
	int tmpfloor = Lift.nowfloor, tmptime = Lift.nowtime;
	int temp = nowfloor;
	
	Man[0].tofloor = Man[1].tofloor;
//	cout << "in send reach nowtime and floor" << nowtime << " " << nowfloor << endl;
	for (int i = 1; i <= N; i ++)
	{
		if ( !Man[i].off && btw( i, nowfloor, tofloor ) )
		{
			Man[i].off = 1;
			cost[ Man[i].tofloor ] = 1;
			cnt ++;
//			cout << "in send reach" << endl;
//			cout << Lift.nowfloor << " " << nowtime << endl;
			Lift.nowfloor = Man[i].tofloor;
			Lift.nowtime += fabs( temp - Man[i].tofloor );
			Lift.Print();
			Man[i].Print();
			Man[i].waitingtime = Lift.nowtime - Man[i].asktime;
			nowtime ++;	//cost of off maybe many off on same floor
			Lift.nowtime ++;
			temp = Man[i - 1].tofloor;
//			cout << "read nowtime lifttime:" << nowtime << " " << Lift.nowtime << endl;
		}
	}
	Lift.nowfloor = tmpfloor;
	Lift.nowtime  = tmptime;
	Man[0].tofloor = 0;
	return cnt;
}

int btw(int i, int nowfloor, int tofloor)
{
	int topf, butf;
	if ( nowfloor >= tofloor )
	{
		topf = nowfloor;
		butf = tofloor;
	}
	else
	{
		topf = tofloor;
		butf = nowfloor;
	}
	
	if ( Man[i].tofloor >= butf && Man[i].tofloor <= topf ) return 1;
	return 0;
}

void slove(int n)
{
	int nowtime = 0, nowfloor = 1, nowdir = 1, tmptime = 0;
	sort( Man, Man + n, cmp );
	while ( !pick.empty() ) pick.pop();
	//ensure the queue is empty

	while (cntprint < N)
	{
		//pick up
		for (int i = 1; i <= N; i ++)
		{
			if ( !Man[i].vis )
			{
				Man[i].vis = 1;
				tmptime += add( i, nowtime, nowfloor );
				find ( nowtime, nowfloor, tmptime );
				pick.push( i );
		
				//pick someone or some up
				while ( !pick.empty() )
				{
					int head;	//the head of pickqueue
					head = pick.front();
							 pick.pop();
//					cout << "pick " << head << endl;
					send.push( head );
					
					Man[head].vis = 1;
					//up
					if ( Lift.nowdir)
					{
//						cout << "nowtime Liftnowtime" << nowtime << " " << Lift.nowtime << endl;
						while ( nowfloor < Man[head].fromfloor)
						{
							nowtime ++;
							nowfloor ++;
							tmptime ++;
							//nowfloor = Man[head].tofloor;
							Lift.nowtime  = nowtime;
							Lift.nowfloor = nowfloor;
					
							//maybe someone arrived his/her destination
							int tmpcntprint = cntprint +
											  arrived( nowfloor, nowtime );
							if ( tmpcntprint != cntprint )
							{
								cntprint = tmpcntprint;
								if ( cntprint == N ) return ;
								//nowtime ++;
								tmptime += nowtime;
								Lift.nowtime = nowtime;
							}
							/*nowtime ++;
							Lift.nowtime = nowtime;
							tmptime ++;*/
						}
//						cout << "after pick while nowtime nowfloor liftnowtime" << nowtime << " " << nowfloor << " " << Lift.nowtime << endl;
					}	//end of if up
					//down						
					else
					{
						while ( nowfloor > Man[head].fromfloor )
						{
							nowtime ++;
							nowfloor --;
							tmptime ++;
							//nowfloor = Man[head].tofloor;
							Lift.nowtime  = nowtime;
							Lift.nowfloor = nowfloor;
					
							//maybe someone arrived his/her destination
							int tmpcntprint = cntprint +
											  arrived( nowfloor, nowtime );
							if ( tmpcntprint != cntprint )
							{
								cntprint = tmpcntprint;
								if ( cntprint == N ) return ;
								//nowtime ++;
								tmptime += nowtime;
								Lift.nowtime = nowtime;
							}
							/*nowtime ++;
							Lift.nowtime ++;
							tmptime ++;*/
						}
					}	//end of else down
					nowtime ++; //pick head up
				}	//end of pick up while
			}	//end of pick up if
		}	//end of pick up for
		//end of pick up
		
//		cout << "time:" << nowtime << " floor:" << nowfloor << endl;
//		cout << "nowdir:" << Lift.nowdir << endl;
		//send someone or some to destination
		while ( !send.empty() )
		{
			int topp;	//the top of sendstack
			topp = send.top();
				   send.pop();
//			cout << "nowtime and floor" << nowtime << " " << nowfloor << " " << send.size() << endl;
			Lift.nowfloor = nowfloor;
			Lift.nowtime  = nowtime;
			
			if ( Man[topp].tofloor > nowfloor ) Lift.nowdir = 1;
			else if ( Man[topp].tofloor < nowfloor ) Lift.nowdir = 0;
			else
			{
//				cout << "in send" << endl;
				Lift.Print();
				Man[topp].Print();
				Man[topp].waitingtime = nowtime - Man[topp].asktime;
				cntprint ++;
				if ( cntprint == N ) return ;
			}
			
			//up	   
			if ( Lift.nowdir)
			{
//				cout << "before call reach" << nowtime << " " << Lift.nowtime << endl;
				int tmpcntprint = cntprint +
								  reach( nowtime, nowfloor, Man[topp].tofloor );
				if ( tmpcntprint != cntprint )
				{
//					cout << "upsend " << nowfloor << " nowprint" << cntprint << endl;
					cntprint = tmpcntprint;
					if ( cntprint == N ) return ;
					nowtime += Man[topp].tofloor - nowfloor;
					Lift.nowtime = nowtime;
					Lift.nowfloor = nowfloor = Man[topp].tofloor;
				}
			}	//end of if up
			//down
			else
			{
				int tmpcntprint = cntprint +
								  reach( nowtime, nowfloor, Man[topp].tofloor );
				if ( tmpcntprint != cntprint )
				{
//					cout << "downsend " << nowfloor << " nowprint" << cntprint << endl;
					cntprint = tmpcntprint;
					if ( cntprint == N ) return ;
					nowtime += nowfloor - Man[topp].tofloor;
					Lift.nowtime = nowtime;
					Lift.nowfloor = nowfloor = Man[topp].tofloor;
				}
			}	//end of else down
		}	//end of send to
	}	//end of while cntprint < N
	
}	//end of slove


int init()
{	
	int flag = 0;
	Man[0].next = 1;
	
	for ( int i = 1; i <= N; i ++ )
	{
		cin >> Man[i].asktime >> Man[i].fromfloor >> Man[i].tofloor;

		Man[i].index = i;
		Man[i].next  = i + 1;		
		if ( Man[i].tofloor == 0 ) Man[i].tofloor = 10;
		
		if ( Man[i].fromfloor > Man[i].tofloor ) Man[i].dir = 0;
		else if ( Man[i].fromfloor < Man[i].tofloor ) Man[i].dir = 1;
		else
		{
			Man[i].vis = 1;
			Lift.Print();
			Man[i].Print();
			cntprint ++;
		}
	}
	Man[N].next = -1;
	if ( cntprint == N )
	{
		flag = 1;
		sum_print();
	}
	return flag;
}

int main()
{
	freopen("input.txt", "r", stdin);
	
	freopen("output.txt", "w", stdout);
	
	if ( init() ) return 0;	//get input information
	else
		{
			slove( N );
		
			sum_print();
		}

	return 0;
	
}
