#include <bits/stdc++.h>
#define N 5
using namespace std;

//Scheme:
//		1.sort depend on asktime
//		(ensure the asktime from tiny to 0x3f3f3f3f...,
//		 or it can't be design to an algorithm)
//		2.slove each man's requirement ( Slove() )
//		(maybe some can combine,but son can't)
//		3.merge each asktime ( Sum() )

struct Man			//	Information of each passenger
{
	int AskTime;	//	require time
	int WaitTime;	//	individual time
	int Value;		//	D-value between form and to
	int D_dir;		//	D-value between lift's direction(now) and fromfloor
	
	int ToFloor; 	//	destination dest-floor 0 == 10, 1 == 1
	int FromFloor; 	//	startfloor 1~10(inclusive?)
	int Index;		//	the input-indexs of each man
}Passenger[N];

bool Flag;			//	whether asktime is enough to cover nowtime
					//	1 == can , 0 contrarily



//Print the answer
void Print(int nowtime, int nowfloor, int index)
{
	printf("On %d, stop on %d floor and Passenger %d arrive.\n", nowtime - 1, nowfloor, index + 1);
}

//The sort cmp of asktime (two keywords: 1.asktime 2.d-value) or D-dir
bool cmp_now(Man a, Man b)
{
	if (a.AskTime != b.AskTime)
		return a.AskTime < b.AskTime;
	else
		return a.Value < b.Value;
}
bool cmp_dir(Man a, Man b)
{
	return a.D_dir < b.D_dir;
}

//Merge each waiting time
int Sum(int n)
{
	int tot = 0;
	for (int i = 0; i < n; i ++)
		tot += Passenger[i].WaitTime;
	return tot;
}

//Find whether the subseauents can travel with nowone or not
int find(int now, int nowtime, int nowfloor)
{
	for (int i = now + 1; i < N; i ++)
		if (Passenger[i].AskTime <= nowtime &&
			Passenger[i].FromFloor == nowfloor) return 1;
	return 0;
}

//All the same
int Easy()
{
	if ((Passenger[0].AskTime == Passenger[1].AskTime &&
		 Passenger[0].AskTime == Passenger[2].AskTime &&
		 Passenger[0].AskTime == Passenger[3].AskTime &&
		 Passenger[0].AskTime == Passenger[4].AskTime)&&
		 
		(Passenger[0].FromFloor == Passenger[1].FromFloor&&
		 Passenger[0].FromFloor == Passenger[2].FromFloor&&
		 Passenger[0].FromFloor == Passenger[3].FromFloor&&
		 Passenger[0].FromFloor == Passenger[4].FromFloor)&&
		 
		(Passenger[0].ToFloor == Passenger[1].ToFloor &&
		 Passenger[0].ToFloor == Passenger[2].ToFloor &&
		 Passenger[0].ToFloor == Passenger[3].ToFloor &&
		 Passenger[0].ToFloor == Passenger[4].ToFloor ) ) return 1;
	
	return 0;
}

//Work out the changing of nowtime
int cnt_nowadd(int i, int nowt, int nowf)
{
	int nowadd = 0;
	
//	nowtime isn't enough to cover nowone's asktime
//	1.asktime isn't enough to cover d-value
//	2.asktime is enough that lift can stop on the nowone's fromfloor on his/her asktime

	if ( nowt <= Passenger[i].AskTime &&
		 Passenger[i].AskTime <= Passenger[i].Value ) nowadd = Passenger[i].Value;
	if ( nowt <= Passenger[i].AskTime &&
		 Passenger[i].AskTime >  Passenger[i].Value ) nowadd = Passenger[i].AskTime;

//	nowtime has exceeded the asktime, so the nowtime should plus the time to pick up nowone
	if ( nowt >  Passenger[i].AskTime)
	{
		nowadd = fabs( nowf - Passenger[i].FromFloor );
		Flag = 1;
	}
		
	return nowadd;
}

//Scheme
void Slove(int n)
{
	int nowfloor = 1, nowtime = 0;
	
	sort(Passenger, Passenger + n, cmp_now);
	

	for (int i = 0; i < n; i ++)
	{
		int  turns = 0;
		
//		Bizarre requirements
		if ( Passenger[i].FromFloor == Passenger[i].ToFloor )
		{
			printf("Sorry, you have already arrived in your destination!\n");
			continue;
		}
		
//		All the same
		if ( Easy() )
		{
			nowtime = 1 + fabs( nowfloor - Passenger[i].FromFloor) + Passenger[i].Value;
			nowfloor = Passenger[i].ToFloor;
			for (int k = 0; k < N; k ++)
			{
//				printf("Easy\n");
				Passenger[k].WaitTime = nowtime;
				Print(nowtime + 1, nowfloor, k);
			}
			return ;
		}
		
		Flag = 0;
		int nowadd = 0;
		nowadd = cnt_nowadd( i, nowtime, nowfloor );
		
		if ( Flag )
		{
			nowtime += nowadd + 1;
		}
		else
		{
//			Pick nowone up
			nowtime = nowadd + 1;
			nowfloor = Passenger[i].FromFloor;
			bool turnsf = 0;
			
//			Work out all the in turns and successively on the same floor
			while ( 1 )
			{
				if ( find( turns, nowtime, nowfloor ) )
				{
					nowtime ++;
					turns ++;
					nowfloor = Passenger[turns].FromFloor;
					turnsf = 1;
				}
				else break;
			}
			
//			All in turns on the same floor
			if (turnsf && turns == N - 1)
			{
				nowfloor = Passenger[i].ToFloor;
				nowtime += fabs( nowfloor - Passenger[i].FromFloor);
				for (int k = 0; k < N; k ++)
				{
					Print(nowtime, nowfloor, Passenger[k].Index);
					Passenger[k].WaitTime = nowtime - 1 - Passenger[k].AskTime;
				}
				return ;
			}
		}
		
//		int Case = 1;
//		printf("%d %d %d\n", Case ++, nowtime, nowfloor);

//		Normal operation
		nowadd = nowtime = 0;
		nowfloor = 1;
		/*dir = 10;
		sort(Passenger + i, Passenger + N + i, cmp_dir);
		
		if ( Passenger[i].ToFloor != dir ) continue;
		dir = Passenger[i].ToFloor;*/
		nowadd = cnt_nowadd( i, nowtime, nowfloor );
		if ( Flag ) nowtime += nowadd + 1;
		else nowtime = nowadd + 1;
		//		Send nowone/they to his/her/their dest
		nowfloor = Passenger[i].ToFloor;
		nowtime += fabs( nowfloor - Passenger[i].FromFloor);
		
		Print( nowtime, nowfloor, Passenger[i].Index );
		//		Ready to merge waiting time
		Passenger[i].WaitTime = nowtime - Passenger[i].AskTime;
		if ( find( i, nowtime - fabs( nowfloor - Passenger[i].FromFloor), 
				   Passenger[i].FromFloor ) ) nowtime --;
		nowtime ++;
	}
}

//Just get the input and work out the value
int main()
{
	for (int i = 0; i < N; i ++)
	{
		scanf("%d%d%d", &Passenger[i].AskTime,
						&Passenger[i].FromFloor,
						&Passenger[i].ToFloor);
		Passenger[i].Index = i;
		Passenger[i].D_dir = 10 - Passenger[i].FromFloor;
		if (! Passenger[i].ToFloor) Passenger[i].ToFloor = 10;
		Passenger[i].Value = fabs( Passenger[i].FromFloor -
								   Passenger[i].ToFloor);	
	}
	
	Slove(N);
	/*for (int i = 0; i < N; i ++)
		printf("%d %d %d\n", Passenger[i].AskTime,
							 Passenger[i].FromFloor,
							 Passenger[i].ToFloor);*/
	printf("The total waiting time: %d s\n", Sum(N));
	
	return 0;
}
