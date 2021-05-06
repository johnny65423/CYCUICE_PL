#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector> 
#include <queue>
using namespace std ;

static fstream file ;

struct Process{
	int id ;
	int cpuburst ;
	int arrivaltime ;
	int priority ;
	
};

bool check(const Process& x, const Process& y){
	if( x.arrivaltime != y.arrivaltime )
		return x.arrivaltime < y.arrivaltime ;
	else 
		return x.id < y.id ;
	
}

struct checkSRTF{
	bool operator() (const Process& x, const Process& y){
		if( x.cpuburst != y.cpuburst )
			return x.cpuburst > y.cpuburst ;
		else {
			if( x.arrivaltime != y.arrivaltime )
				return x.arrivaltime > y.arrivaltime ;
			else 
				return x.id > y.id ;
		}
	}
};


char getid( int num ) {
	if ( num > 35 || num < 0 )
		return '#' ;
	else if ( num < 10 )
		return num + 48 ;
	else 
		return num + 55 ;	

}

class Scheduler {
	public:
		void readfile(){
		  string info ;
			file >> method ;
			file >> timeslice ;
			// cout << method << " " << timeslice << endl ;
			getline( file, info ) ;
			getline( file, info ) ;

			while( !file.eof() ) {
				readprocess() ;
			}	

		}
		
		void setfile(){
			// sort(processlist.begin(),processlist.end(), check) ;
			
			for ( int i = 0 ; i < processlist.size() ; i++ ) {
				cout << processlist.at(i).id << "\t" ;
				cout << processlist.at(i).cpuburst << "\t" ;
				cout << processlist.at(i).arrivaltime << "\t" ;
				cout << processlist.at(i).priority << endl ;
			}
		}	

		void makeschedule(){
			// cout << method ;
			switch ( method ) {
				case 1 :
					FCFS() ;
				case 2 :
					RR() ;
				case 3 :
					SRTF();
				case 6 :
					SRTF() ;	
				/*
				
				case 4 :
					PPRR();
				case 5 :
					HRRN();
				case 6 :
					allmethod() ;
				*/
			
			} // switch
			
		}

	private:
		vector<Process> processlist ;
		int method ;
		int timeslice ;
		
		void readprocess() {
			Process temp ;
			
			file >> temp.id >> temp.cpuburst >> temp.arrivaltime >> temp.priority ;
			if (!file.eof())
				processlist.push_back( temp ) ;
			
		}	

		void FCFS(){
			cout << "FCFS" << endl ;
			int time = 0 ;
			vector< Process > list ;
			list.assign( processlist.begin(), processlist.end() );
			sort( list.begin(),list.end(), check ) ;
			queue<Process> pqueue ;
			int nowwork = -1 ;
			int remaindertime = 0 ;
			while ( !pqueue.empty() || !list.empty() ) {
				for ( int i = 0 ; i < list.size() ; i++ ) {
					if( list.at(i).arrivaltime == time ) {
						pqueue.push( list.at(i) ) ;
						list.erase(list.begin() + i) ;
						i = -1 ;
					}
				}
				
				if( remaindertime == 0 && !pqueue.empty() ) {
					Process temp = pqueue.front() ;
					pqueue.pop() ;
					remaindertime = temp.cpuburst ;	
					nowwork = temp.id ;
				}
				else if( remaindertime == 0 && pqueue.empty() )
					nowwork = -1 ;
					
				if( nowwork != -1 )
					cout << getid(nowwork) ;
				else 
					cout << '-' ;	

				time++ ;
				if( nowwork != -1 )
					remaindertime-- ;
			}

			while( remaindertime > 0 ) {
				if( nowwork != -1 )
					cout << getid(nowwork) ;
				else 
					cout << '-' ;	
				remaindertime-- ;
			}

		}
		
		void RR(){
			cout << "RR" << endl ;
			int time = 0 ;
			vector< Process > list ;
			list.assign( processlist.begin(), processlist.end() );
			sort( list.begin(),list.end(), check ) ;
			queue<Process> pqueue ;
			int nowwork = -1 ;
			int remaindertime = 0 ;
			int runtime = 0 ;
			while ( !pqueue.empty() || !list.empty() ) {
				for ( int i = 0 ; i < list.size() ; i++ ) {
					if( list.at(i).arrivaltime == time ) {
						pqueue.push( list.at(i) ) ;
						list.erase(list.begin() + i) ;
						i = -1 ;
					}
				}
				// cout << runtime << " " << timeslice << " " << remaindertime << endl ;
				if( runtime == timeslice && remaindertime != 0 ) {
					// cout << "cut" << endl ;
					Process temp ;
					temp.id = nowwork ;
					temp.cpuburst = remaindertime ;
					pqueue.push( temp ) ;
					runtime = 0 ;
					temp = pqueue.front() ;
					pqueue.pop() ;
					remaindertime = temp.cpuburst ;	
					nowwork = temp.id ;
				}	
				
				if( remaindertime == 0 && !pqueue.empty() ) {
					Process temp = pqueue.front() ;
					pqueue.pop() ;
					remaindertime = temp.cpuburst ;	
					nowwork = temp.id ;
					runtime = 0 ;
				}
				else if( remaindertime == 0 && pqueue.empty() )
					nowwork = -1 ;
					
				if( nowwork != -1 )
					cout << getid(nowwork) ;
				else 
					cout << '-' ;	

				time++ ;
				runtime++ ;
				if( nowwork != -1 )
					remaindertime-- ;
			}

			while( remaindertime > 0 ) {
				if( nowwork != -1 )
					cout << getid(nowwork) ;
				else 
					cout << '-' ;	
				remaindertime-- ;
			}

		}
		
		void SRTF(){
			cout << "SRTF" << endl ;
			int time = 0 ;
			vector< Process > list ;
			list.assign( processlist.begin(), processlist.end() );
			sort( list.begin(),list.end(), check ) ;
			// queue<Process> pqueue ;
			priority_queue<Process, vector<Process>, checkSRTF > pqueue;
			int nowwork = -1 ;
			int arrtime ;
			int remaindertime = 0 ;
			while ( !pqueue.empty() || !list.empty() ) {
				for ( int i = 0 ; i < list.size() ; i++ ) {
					if( list.at(i).arrivaltime == time ) {
						pqueue.push( list.at(i) ) ;
						list.erase(list.begin() + i) ;
						i = -1 ;
					}
				}
				if( !pqueue.empty() )
					cout << "*" << pqueue.size() << endl;
				if( !pqueue.empty() && nowwork != -1 && remaindertime > pqueue.top().cpuburst ) {
					// cout << "cut" << endl ;
					Process temp ;
					temp.id = nowwork ;
					temp.cpuburst = remaindertime ;
					temp.arrivaltime = arrtime ;
					pqueue.push( temp ) ;
					temp = pqueue.top() ;
					pqueue.pop() ;
					remaindertime = temp.cpuburst ;	
					nowwork = temp.id ;
				}	
				
				if( remaindertime == 0 && !pqueue.empty() ) {
					Process temp = pqueue.top() ;
					pqueue.pop() ;
					remaindertime = temp.cpuburst ;	
					arrtime = temp.arrivaltime ;
					nowwork = temp.id ;
				}
				else if( remaindertime == 0 && pqueue.empty() )
					nowwork = -1 ;
					
				if( nowwork != -1 )
					cout << getid(nowwork) ;
				else 
					cout << '-' ;	

				time++ ;
				if( nowwork != -1 )
					remaindertime-- ;
			}

			while( remaindertime > 0 ) {
				if( nowwork != -1 )
					cout << getid(nowwork) ;
				else 
					cout << '-' ;	
				remaindertime-- ;
			}

		}
} ;

int main() {
	Scheduler scheduler = Scheduler() ;
	string filename ;
	int method ;
	int timeslice ;
	priority_queue<Process, vector<Process>, checkSRTF > pqueue;
	Process temp ;
	temp.id = 6 ;
  	temp.cpuburst = 5 ;
	temp.arrivaltime = 1 ;
	pqueue.push(temp) ;
	temp.id = 9 ;
  	temp.cpuburst = 4 ;
	temp.arrivaltime = 1 ;
	pqueue.push(temp) ;
	
	cout << pqueue.top().id ;
	cout << pqueue.top().id ;
	
} // main()	


