#include "elevator.h"
#include <bits/stdc++.h>
using namespace std;


//CLASS ELEVATOR

Elevator:: Elevator()
{
	nowfloor = nowdir = 1;
	nowtime = sumtime = 0;
}
Elevator:: ~Elevator()
{}

void Elevator:: Print()
{
	cout << "On " << nowtime << ",stop on " << nowfloor << ". ";
}




//CLASS PASSENGER

Passenger:: Passenger()
{
	vis = waitingtime = off = 0;
}
Passenger:: ~Passenger()
{}

void Passenger:: Print()
{
	cout << "Passenger " << index << " arrives." << endl;
}

int Passenger:: judge()
{
	if ( vis == 1 ) return 1;
	return 0;
}
