#include <reg51.h>

//IO Pins
sbit doorLed = P2^0;			//Door Open-Close
sbit up = P2^2;						//Up Led
sbit down = P2^1;					//Down Led

sbit loc_4 = P2^3;				//Location Indicators
sbit loc_3 = P2^4;
sbit loc_2 = P2^5;
sbit loc_1 = P2^6;

sbit u1 = P1^0;						//Buttons At each Level
sbit u2 = P1^1;						//For Up as well as down
sbit u3 = P1^2;
sbit u4 = P1^3;
sbit d1 = P1^4;		
sbit d2 = P1^5;		
sbit d3 = P1^6;
sbit d4 = P1^7;

sbit s1 = P0^3;						//Butons inside at Lift
sbit s2 = P0^2;
sbit s3 = P0^1;
sbit s4 = P0^0;	
sbit doorClose = P0^4;		//To sense if Door of Elevator is closer or not

sbit e = P3^2; 						//Emergency Button in Lift it will be Handeled by Intrupt it will be Highest Priority Intrupt
sbit DS = P3^3;						//This will be simulating Door Sensor Button

//Local Variables 
int i;													//For {for loops}
int LocationOfElevator = 1;			//Memory to store Location od Elevator
char req[2];										//Memory to store and Request
int reqForLevel2Go[4];					//Memory to store request Inside for every level NO=0, YES=1
int upRequestsOutside[4];				//Memory to store up request Outside for every level NO=0, YES=1
int downRequestsOutside[4];			//Memory to store down request Outside for every level NO=0, YES=1
int direction = 1;							//Memory for Direction. Up=1, Down=0

//Implementation of Functions
/*
Almost all of self explainator due to their names
But i have all so given explaination where required in functions through comments 
*/
void reset (void){
	//This Function resets by Forcefully seting instruction pointer on to 0000 HEX
	((void (code *) (void)) 0x0000) ();
}

void delay(int time){			
	//Delay Function
	int i,j;
	for(i=0;i<time;i++)
		for(j=0;j<1275;j++);
}

void upLed(int u){
	//On = 1, Off = 0 
	up = u;
	return;
}

void downLed(int d){
	//On = 1, Off = 0
	down = d;
	return;
}

void doorIsClosed(void){			//this function will not return Control Until Door is Closed
		doorLed = 1;							//Switch on Door_Open Led
		while(1){
			if(doorClose == 1){
				doorLed = 0;					//Switch off Door_Open Led
				return;
		}
	}
}

void where2Go(void){
	/*
	This functions monitor requests on Inside of Elevator and store them
	in Local Memory/Variables
	Yes = 1
	No = 0
		
	These Local Variables are always being monitored
	*/
	while(1){
		if(s1 == 1){
			reqForLevel2Go[0] = 1;
			return;
		}
		if(s2 == 1){
			reqForLevel2Go[1] = 1;
			return;
		}
		if(s3 == 1){
			reqForLevel2Go[2] = 1;
			return;
		}
		if(s4 == 1){
			reqForLevel2Go[3] = 1;
			return;
		}
	}
}

void location(int L){
	/*
	This Functions will be managing LED related to Locatuion of Elevator's current location
	Also this function will be taking decesion on basis of of following variable:
		
	reqForLevel2Go				----->Requests from inside of Elevator s1... 
	upRequestsOutside 		----->Requests from outside of Elevator u1...
	downRequestsOutside		----->Requests from outside of Elevator d1...
	
	Also this function will be updating values on memory
	*/
	delay(50);
	
	loc_1=0;
	loc_2=0;
	loc_3=0;
	loc_4=0;
	
	if(L== 1){											//Level 1
		loc_1 = 1;
		
		if(reqForLevel2Go[0]){				//Handeling inside Request of Level 1
			reqForLevel2Go[0] = 0;			//Reset Request to NO
			doorIsClosed();
			where2Go();
		}
		
		if(upRequestsOutside[0] && direction == 1){
			doorIsClosed();
			where2Go();
			upRequestsOutside[0] = 0;				//Reset Request to NO
		}
	}		

	if(L== 2){											//Level 2
		loc_2 = 1;
		
		if(reqForLevel2Go[1]){				//Handeling inside Request of Level 2
			reqForLevel2Go[1] = 0;			//Reset Request to NO
			doorIsClosed();
			where2Go();
		}
		
		if(upRequestsOutside[1] && direction == 1){
			doorIsClosed();
			where2Go();
			upRequestsOutside[1] = 0;				//Reset Request to NO
		}
		
		if(downRequestsOutside[1] && direction == 0){
			doorIsClosed();
			where2Go();
			downRequestsOutside[1] = 0;
		}
	}	

	if(L== 3){											//Level 3
		loc_3 = 1;
		
		if(reqForLevel2Go[2]){				//Handeling inside Request of Level 2
			reqForLevel2Go[2] = 0;			//Reset Request to NO
			where2Go();
			doorIsClosed();
		}
		
		if(upRequestsOutside[2] && direction == 1){
			doorIsClosed();
			where2Go();
			upRequestsOutside[2] = 0;				//Reset Request to NO
		}
		
		if(downRequestsOutside[2] && direction == 0){
			doorIsClosed();
			where2Go();
			downRequestsOutside[2] = 0;
		}
	}	
	
	if(L== 4){											//Level 4
		loc_4 = 1;
		
		if(reqForLevel2Go[3]){				//Handeling inside Request of Level 4
			reqForLevel2Go[3] = 0;			//Reset Request to NO
			doorIsClosed();
			where2Go();
		}
		
		if(downRequestsOutside[3] && direction == 0){
			doorIsClosed();
			where2Go();
			downRequestsOutside[3] = 0;
		}
	}	
}

bit ifRequestOutside(void){	//Check if there is Request on any u1... or d1...
	while(1){
		if(u1 == 1){
		req[0] = 'u';
		req[1] = '1';
		return 1;
		}
		if(u2 == 1){
		req[0] = 'u';
		req[1] = '2';
		return 1;
		}	
		if(u3 == 1){
		req[0] = 'u';
		req[1] = '3';
		return 1;
		}
		if(u4 == 1){
		req[0] = 'u';
		req[1] = '4';
		return 1;
		}
		if(d1 == 1){
		req[0] = 'd';
		req[1] = '1';
		return 1;
		}
		if(d2 == 1){
		req[0] = 'd';
		req[1] = '2';
		return 1;
		}	
		if(d3 == 1){
		req[0] = 'd';
		req[1] = '3';
		return 1;
		}
		if(d4 == 1){
		req[0] = 'd';
		req[1] = '4';
		return 1;
		}
	}
}

void emergency(void)interrupt 0{
	
	location(LocationOfElevator);
	upLed(0);
	downLed(0);
	doorLed = 1;		//Open Door
	delay(1000);			//10 minutes Delay and then Elevator Reset's
	reset();
	
	return;
}	

void dsPressed(void) interrupt 2{			          	//DS
	/*
	Functionality of dsPressed()
	1.It Checks if LocationOfElevator doesn't overflows i.e LocationOfElevator should always remain b/t 1 to 4
	2.In case LocationOfElevator is at Limit function will go to change directions of Elevator to up/down accordingly
	
	3.After all these checks our Elevator will move up/down accordingly
	4.Now location() will be called which wil be handling LED's
	
	
*/	
	if(LocationOfElevator == 1){
		direction = 1;							
	}
	if(LocationOfElevator == 4){
		direction = 0;							
	}
	
	//Both of these condition are fulfilling functionality till (2)	
	
	if(direction == 1){							//When Elevator is going Up
		LocationOfElevator++;
		downLed(0);
		delay(5);
		upLed(1);
		location(LocationOfElevator);
	}
	
	if(direction == 0){							//When Elevator is going Down
		LocationOfElevator--;
		upLed(0);
		delay(5);
		downLed(1);
		location(LocationOfElevator);
	}
	return;
}

void interruptsManager(int Mode){
	/*
	As i have been using two interrupts EX0, EX1
	EX0   ---> Emergency Button This interrupt should always be on
	EX1   ---> DS this interrupt will be on/off when needed
	
	There will be two mode's 
	0. Only Emergency
	1. Both Emergency and DS
	*/
	
	switch(Mode){
		case 0:
			IE = 0x81; 
			break;
		case 1:
			IE = 0x85;
			break;
	}
	return;
}

void requestHandler(void){
	interruptsManager(1);			//interrupts in Mode 1 for refference see details in Function
	if((req[0] == 'u' && req[1] == '1') == 1){		//Request U1
		if(LocationOfElevator == 1){
			doorIsClosed();
			where2Go();
		}
	}
		
	if((req[0] == 'u' && req[1] == '2') == 1){		//Request U2
			upRequestsOutside[1] = 1;	//Set request to YES
	}
		
	if((req[0] == 'u' && req[1] == '3') == 1){		//Request U3
		upRequestsOutside[2] = 1;	//Set request to YES
	}
		
	if((req[0] == 'u' && req[1] == '4') == 1){		//Request U4
		upRequestsOutside[3] = 1;	//Set request to YES
	}
		
	if((req[0] == 'd' && req[1] == '1') == 1){		//Request D1
		downRequestsOutside[0] = 1;			//Set request to YES
	}
		
	if((req[0] == 'd' && req[1] == '2') == 1){		//Request D2
		downRequestsOutside[1] = 1;			//Set request to YES
	}
		
	if((req[0] == 'd' && req[1] == '3') == 1){		//Request D3
		downRequestsOutside[2] = 1;			//Set request to YES

	}
		
	if((req[0] == 'd' && req[1] == '4') == 1){		//Request D4
		downRequestsOutside[3] = 1;			//Set request to YES
	}
	
	return;
}


void setup(void){
	P0 = 0xFF;
	P1 = 0xFF;
	P2 = 0xFF;
	P3 = 0xFF;
	
	delay(100);
	
	P0 = 0x00;					//Clearing Pins to Monitor s1...
	P1 = 0x00;					//Clearing Pins to Monitor u1... and d1...
	P2 = 0x00;					//Clear Everything...
	//P3 = 0x00;
	LocationOfElevator = 1;
	location(1);				//Set Location to First Floor
	direction = 1;			//Set Dirrection to up

	for(i=0;i<4;i++){
		reqForLevel2Go[i] = 0;		//Set all inside request to 'NO' 
		upRequestsOutside[i];			//Set all outside up request to 'NO' 
		downRequestsOutside[i];		//Set all outside down request to 'NO'
	}

	interruptsManager(0);			//interrupts in Mode 0 for refference see details in Function
	IP = 0x01;								//Interrupt Priority
	return;
}

void loop(void){
	if(ifRequestOutside() == 1){
		requestHandler();
	}
	return;
}


void main(){
	setup();
	while(1){
		loop();
	}	
	return;
}	
