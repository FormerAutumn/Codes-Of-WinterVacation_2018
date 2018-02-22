class Elevator
{
	public:
					int nowfloor;
					int nowtime;
					int nowdir;
					int sumtime;

					Elevator();
					~Elevator();
					void Print();
};




class Passenger
{
	public:
					int asktime;
					int fromfloor;
					int tofloor;
					int dir;			// 1 == up, 0 == down
					int waitingtime;
					int index;
					int vis;			// 1 == on, 0 == unon
					int off;			// 1 == off 0 == unoff
					int next;			// use a num to simulate list
					
					Passenger();
					~Passenger();
					int judge();
					void Print();
};
