// C++ program to implement Shortest Remaining Time First
#include <bits/stdc++.h>
#include <queue>
using namespace std;

struct Process {
	int pid,at,bt,ft,rpt,wt,tat;	
};

void fcfs(Process p[], int n){
   	for(int i=0;i<n-1;i++){
		for(int j=0;j<n-i-1;j++){
			if(p[j].at>p[j+1].at){
				int temp;
				//sorting burst times
				temp = p[j].bt;
				p[j].bt = p[j+1].bt;
				p[j+1].bt = temp;
				//sorting arrival times
				temp = p[j].at;
				p[j].at = p[j+1].at;
				p[j+1].at = temp;
				//sorting their respective IDs
				temp = p[j].pid;
				p[j].pid = p[j+1].pid;
				p[j+1].pid = temp;
			}
		}
	}
     p[0].ft=p[0].at+p[0].bt;p[0].rpt=0;
 for(int i=1;i<n;i++){
    p[i].ft=p[i-1].ft+p[i].bt;
	p[i].rpt=p[i].ft-p[i].at-p[i].bt;
 }
}

void sjf(Process proc[], int n) {
   int rt[n];
   for (int i = 0; i < n; i++)
   rt[i] = proc[i].bt;
   int complete = 0, t = 0, minm = INT_MAX;
   int shortest = 0;
   bool check = false;
   while (complete != n) {
      for (int j = 0; j < n; j++) {
         if ((proc[j].at <= t) && (rt[j] < minm) && rt[j] > 0) {
            minm = rt[j];
            shortest = j;
            check = true;
         }
      }
      if (check == false) {
         t++;}
               
      else{
      rt[shortest]=0;
      minm = INT_MAX;
      complete++;
     t= t + proc[shortest].bt;
      proc[shortest].ft = t; 
	  proc[shortest].rpt=proc[shortest].ft-proc[shortest].at-proc[shortest].bt;     
      } 
   }
}

void srtf(Process proc[], int n){
	int rt[n];

	// Copy the burst time into rt[]
	for (int i = 0; i < n; i++)
		rt[i] = proc[i].bt;

	int complete = 0, t = 0, minm = INT_MAX;
	int shortest = 0;
	bool check = false;
    bool entry[n]={false};
	// Process until all processes gets completed
	while (complete != n) {

		// Find process with minimum remaining time among the
		// processes that arrives till the current time`
		for (int j = 0; j < n; j++) {
			if ((proc[j].at <= t) &&
			(rt[j] < minm) && rt[j] > 0) {
				minm = rt[j];
				shortest = j;
				check = true;
			}
		}

		if (check == false) {
			t++; continue;
		}
		if(!entry[shortest]){
			proc[shortest].rpt=t - proc[shortest].at;
			entry[shortest]=true;
		}
		rt[shortest]--;  // Reduce remaining time by one
		minm = rt[shortest]; // Update minimum
		
	// If a process gets completely executed
		if (rt[shortest] == 0) {
            minm = INT_MAX;
			complete++;   // Increment complete
			check = false;

			// Find finish time of current process
			proc[shortest].ft = t + 1;			
		}
			t++; 	// Increment time
	}
}

void rr(Process proc[],int n)
{
    queue<int> q1;
	bool entry[n]={false};
	int i=0, ts, m, nextval, nextarr;
	nextval = proc[0].at;
    int rt[n];
	
    // Copy the burst time into rt[]
	for (int i = 0; i < n; i++)
		rt[i] = proc[i].bt;

	cout << "\nEnter the Time Slice or Quantum: ";
	cin >> ts;

	for (i = 0; i < n && proc[i].at <= nextval; i++)
			q1.push(i);
	
	while (!q1.empty())
	{
      	m = q1.front();
		if(!entry[m]){ 
           proc[m].rpt= nextval-proc[m].at;  entry[m]=true;
		}
		q1.pop();

		if (rt[m] >= ts)
		{
			nextval = nextval + ts;
            rt[m] = rt[m]  - ts;
		}
		else
		{
			nextval = nextval + rt[m] ;
            rt[m]  = 0;
		}
		while (i <= n && proc[i].at <= nextval)
		{
			q1.push(i++);	
		}

		if (rt[m] > 0)
			q1.push(m);
		else proc[m].ft = nextval;
	}
}

void priority_p(Process proc[], int n){
    int pt[n], bst[n];
    cout << "\nEnter the priorities for " << n << " jobs: ";
    for(int i=0;i<n;i++)
        cin >> pt[i];

    for(int i=0;i<n;i++) 
        bst[i] = proc[i].bt;   // Copy the burst time into bst[]
        
    int count = 0, t = 0, high_p = 0, highest = 0;
    bool check = false;

    // Process until all processes get completed
    while (count != n) {

        // Find the process with the highest priority and minimum remaining time
        for (int j = 0; j < n; j++) {
            if ((proc[j].at <= t) &&
                (pt[j] > high_p) &&
                bst[j] > 0) {
                high_p = pt[j];
                highest = j;
                check = true;
            }
        }
        t++;

        if (check == true) {
            bst[highest]--;
            check = false;
            if (bst[highest] == 0) {
                count++;
                pt[highest] = 0;
                proc[highest].ft = t;
            }
        }
        high_p = 0;
    }
}

void priority_np(Process p[], int n){
	int x[n];
	printf("\nEnter the priorities for %d jobs: ",n);
	for(int i=0;i<n;i++)
     cin>>x[i];
	int time=0,highest=0,count=0;
   
      while(count!=n) {
          for(int i=0;i<n;i++)
        {
            if(p[i].at<=time && x[i]>x[highest] && x[i]>0 )
                highest=i;
        }
        time+=p[highest].bt;
        x[highest]=0;
        count++;
            p[highest].ft=time;
   }
}

void findavgTime(Process proc[], int n)  // Function to calculate average time
{
	int total_wt = 0,total_tat = 0,total_rpt = 0;
// Display processes along with all details
	cout << " P\t\t"
		<< "AT\t\t"
		<< "BT\t\t"
		<< "FT\t\t"
		<< "WT\t\t"
		<< "RT\t\t"
		<< "TAT\t\t\n";

// Calculate turn around time and waiting time	
    for (int i = 0; i < n; i++){
          proc[i].tat = proc[i].ft -proc[i].at;
         proc[i].wt = proc[i].tat-proc[i].bt ;
    }
  
  // Calculate total waiting time and total turnaround time
	for (int i = 0; i < n; i++) {
		total_wt = total_wt + proc[i].wt;
		total_tat = total_tat + proc[i].tat;
		total_rpt = total_rpt + proc[i].rpt;
		cout << " " << proc[i].pid << "\t\t" << proc[i].at << "\t\t "
			<< proc[i].bt << "\t\t "<< proc[i].ft << "\t\t " <<proc[i].wt << "\t\t " <<proc[i].rpt
			<< "\t\t " << proc[i].tat << endl;
	}

	cout << "\nAverage waiting time = "
		<< (float)total_wt / (float)n;
		cout << "\nAverage turn around time = "
		<< (float)total_tat / (float)n ;
	cout << "\nAverage response time = "
		<< (float)total_rpt / (float)n << endl;
}

// Driver code
int main()
{
    int n;cout<<"enter no. of processes: ";cin>>n;
	Process proc[n];
    cout<<"enter the process_id,arrival time and burst time :\n";
	for(int i=0;i<n;i++){
    cout<<"for job"<<i+1<<": ";
    cin>> proc[i].pid >> proc[i].at >> proc[i].bt;
    }
	int ch;
while(ch!=7){
cout<<"\n1. First Come First Serve (FCFS)\n2. Shortest Job First (SJF)\n3. Shortest remaining Job First (SRTF)\n4. Round Robin (RR)\n5. Priority non preemptive\n6. Priority Prepemptive.\n7. Exit\n";
 cout<<"\nenter the choice: "; cin>>ch;
switch(ch){
      case 1:
          fcfs(proc, n);break;
      case 2:
          sjf(proc, n); break;
      case 3:
           srtf(proc, n);break;
      case 4:
           rr(proc, n); break;
      case 5: 
	      priority_np(proc,n); break;
      case 6:
         priority_p(proc, n);break;    
      case 7:exit(0);    
      default: cout<<"enter the valid entry\n";
}
 findavgTime(proc, n);
}
	return 0;
}