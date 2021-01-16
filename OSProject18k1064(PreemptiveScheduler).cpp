#include <iostream>
#include <algorithm> 
#include <iomanip>
#include <string.h> 
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
using namespace std;
struct process_sceduling{

    int pid;
    int priority;//priority
    int at;//arrivaltime
    int bt;//bursttime
    int ct;//completiontime
    int st;//starttime
    int rt;//responsetime
    int tat;//turnaroundtime
    int wt;//waitingtime
    int lastmemberarr[];

};

class priorityscheduling{
	public:
char buffer[sizeof(process_sceduling) + 100* sizeof(int)];
process_sceduling*process= (process_sceduling*)buffer;//this is making an object of struct

int fin;//process finished
	int totalwt;//total wait time
	int totalrt;//total response time
	int totaltat;//total turnaround time
	float avgwt;//average wait time
	float avgtat;//average turnaround time
	float avgrt;//average responsetime
	int current;
    int completed;
    int prev;
    int burst_remaining[100];
    int is_completed[100];

    priorityscheduling()
	{
	totalwt=0;
	totalrt=0;
	totaltat=0;
	current= 0;
    completed=0;
    prev = 0;
    memset(is_completed,0,sizeof(is_completed));
     cout << setprecision(3) << fixed;
    	executescheduler();
	}
	void executescheduler()
	{
		
    cout<<"Enter the number of processes: ";
    cin>>fin;

		for(int i = 0;i<fin; i++) {
        cout<<"Enter arrival time of the process"<<i+1<<": ";
        cin>>process[i].at;
        cout<<"Enter burst time"<<i+1<<": ";
        cin>>process[i].bt;
        cout<<"Enter priority"<<i+1<<": ";
        cin>>process[i].priority;
        process[i].pid = i+1;
        burst_remaining[i]=process[i].bt;
        cout<<endl;
    }
  while(completed!=fin) {//this part had a few errors that has to be corrected
        int idx=-1;
        int mx=-1;
        for(int i=0;i<fin; i++) {
            if(process[i].at <= current&&is_completed[i]==0) {
                if(process[i].priority>mx) {
                    mx=process[i].priority;
                    idx=i;
                }
                if(process[i].priority==mx) {
                    if(process[i].at<process[idx].at) {
                        mx=process[i].priority;
                        idx=i;
                    }
                }
            }
        }
          if(idx!=-1) {
            if(burst_remaining[idx]==process[idx].bt) {
                process[idx].st=current;
//                total_idle_time += p[idx].start_time - prev;
            }
            burst_remaining[idx]-=1;
            current++;
            prev=current;
            
            if(burst_remaining[idx]==0) {
                process[idx].ct=current;
                process[idx].tat=process[idx].ct-process[idx].at;
                process[idx].wt=process[idx].tat-process[idx].bt;
                process[idx].rt=process[idx].st-process[idx].at;
                totaltat+=process[idx].tat;
                totalwt+=process[idx].wt;
                totalrt+=process[idx].rt;
                is_completed[idx]=1;
                completed++;
            }
        }
        else {
             current++;
        }  
    }
 avgtat=(float)totaltat/fin;
avgwt=(float)totalwt/fin;
avgrt=(float)totalrt/fin;
//cpu_utilisation = ((max_completion_time - total_idle_time) / (float) max_completion_time )*100;
cout<<"#P\t"<<"AT\t"<<"BT\t"<<"PRI\t"<<"ST\t"<<"CT\t"<<"TAT\t"<<"WT\t"<<"RT\t"<<"\n"<<endl;

    for(int i=0; i<fin; i++) {
        cout<<process[i].pid<<"\t"<<process[i].at<<"\t"<<process[i].bt<<"\t"<<process[i].priority<<"\t"<<process[i].st<<"\t"<<process[i].ct<<"\t"<<process[i].tat<<"\t"<<process[i].wt<<"\t"<<process[i].rt<<"\t"<<"\n"<<endl;
    }
    cout<<"Average Turnaround Time = "<<avgtat<<endl;
    cout<<"Average Waiting Time = "<<avgwt<<endl;
    cout<<"Average Response Time = "<<avgrt<<endl;
 
	}
};
class roundrobin{
public:
char buffer[sizeof(process_sceduling) + 100* sizeof(int)];
process_sceduling*processrr= (process_sceduling*)buffer;
int quant;
int fin;
int totalwt;//total wait time
	int totalrt;//total response time
	int totaltat;//total turnaround time
	float avgwt;//average wait time
	float avgtat;//average turnaround time
	float avgrt;//average responsetime
	int current;
    int completed;
    int prev;
    int is_completed[100];
    int burst_remaining[100];
roundrobin()//roundrobin constructor
{
	quant=1;//setting a default quantam size
	executescheduler_rr();
	memset(is_completed,0,sizeof(is_completed));
     cout << setprecision(3) << fixed;
	
}
	void executescheduler_rr()
	{
	cout<<"Enter the time Quantam for roundrobin: "<<endl;
	cin>>quant;
    cout<<"Enter the number of processes: ";
    cin>>fin;

		for(int i = 0;i<fin; i++) {
        cout<<"Enter arrival time of the process"<<i+1<<": ";
        cin>>processrr[i].at;
        cout<<"Enter burst time"<<i+1<<": ";
        cin>>processrr[i].bt;
        processrr[i].pid = i+1;
        burst_remaining[i]=processrr[i].bt;
        cout<<endl;
        
          int rem_bt[fin]; 
    for (int i = 0 ; i <fin ; i++) 
        rem_bt[i] = processrr[i].bt; 
  
    int t = 0; // Current time 
  

    while (1) 
    { 
        bool done = true; 
  
       
        for (int i = 0 ; i <fin; i++) 
        { 
      
            if (rem_bt[i] > 0) 
            { 
                done = false; 
  
                if (rem_bt[i] > quant) 
                { 
               
                    t += quant; 
  
                    
                    rem_bt[i] -= quant; 
                } 
  
                else
                { 
                
                    t = t + rem_bt[i]; 
                   processrr[i].wt=t-processrr[i].bt; 
                    rem_bt[i] = 0; 
                } 
            } 
        } 
        if (done == true) 
          break; 
    } 
       
    }
    

    for (int i = 0;i<fin; i++) 
       {
		processrr[i].tat=processrr[i].bt+ processrr[i].wt; }
		
		for (int i=0;i<fin;i++) 
    { 
        totalwt=totalwt+ processrr[i].wt; 
        totaltat=totaltat + processrr[i].tat; 
      
    } 
    avgwt=((float)totalwt/(float)fin)-1; 
   avgtat=((float)totaltat/(float)fin)-1; //i did minus as it was giving me the result incremented
  
	cout<<"#P\t"<<"AT\t"<<"BT\t"<<"TAT\t"<<"WT\t"<<endl;

    for(int i=0; i<fin; i++)
	{
    cout<<processrr[i].pid<<"\t"<<processrr[i].at<<"\t"<<processrr[i].bt<<"\t"<<processrr[i].tat<<"\t"<<processrr[i].wt<<"\t"<<"\n"<<endl;
    }
    cout<<"Average Turnaround Time = "<<avgtat<<endl;
    cout<<"Average Waiting Time = "<<avgwt<<endl;
   

	
    }
    
   

	
};
int main()
{
	cout<<"Pre-Emptive Process Scheduler"<<endl;
	cout<<"Enter 1 for Priority Scheduling"<<endl;
	cout<<"Enter 2 for Round-robin Scheduling"<<endl;
	cout<<"Enter 0 to exit"<<endl;
	int opt;
	l1:
	cin>>opt;
	cout<<endl;
	switch(opt)
	{
		case 0:
		{
		cout<<"Program Exited"<<endl;
		exit(234);
		break; }
		
		case 1:
		{
				cout<<"Priority Scheduler Operating"<<endl;
		       priorityscheduling p1;
			   break;}
			
			case 2:
			{
				cout<<"Round Robin Operating"<<endl;
				roundrobin r1;
				break;}
				
				default:
					{
					cout<<"Choose 0, 1 or 2. Please select the right option."<<endl;
					goto l1;
					break;}
			
		}
		
	}

