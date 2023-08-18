#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

struct Process
{
    int pid;
    float arrivalTime;
    float burstTime;
    float burstTimeRemaining;
    float completionTime;
    float turnaroundTime;
    float waitingTime;
    float Enter;
    float responseTime;
    bool isComplete;
    bool inQueue;
    bool in;
};

void remove(int t, queue<int> &q)
{

    // Helper queue to store the elements
    // temporarily.
    queue<int> ref;
    int s = q.size();
    int cnt = 0;

    // Finding the value to be removed
    while (q.front() != t && !q.empty())
    {
        ref.push(q.front());
        q.pop();
        cnt++;
    }

    // If element is not found
    if (q.empty())
    {
        // cout << "element not found!!" << endl;
        while (!ref.empty())
        {

            // Pushing all the elements back into q
            q.push(ref.front());
            ref.pop();
        }
    }

    // If element is found
    else
    {
        q.pop();
        while (!ref.empty())
        {

            // Pushing all the elements back into q
            q.push(ref.front());
            ref.pop();
        }
        int k = s - cnt - 1;
        while (k--)
        {

            // Pushing elements from front of q to its back
            int p = q.front();
            q.pop();
            q.push(p);
        }
    }
}

/*
Checking for any new arrivals and push them into the queue
* After every time quantum
* When a process has been executed before the time quantum,
* Every time an SJF process has been executed
*/
void checkForNewArrivals(Process processes[], const int n, float currentTime, queue<int> &readyQueue)
{
    for (int i = 0; i < n; i++)
    {
        Process p = processes[i];
        // checking if any processes has arrived
        // if so, push them in the ready Queue.
        if (p.arrivalTime <= currentTime && !p.inQueue && !p.isComplete)
        {
            processes[i].inQueue = true;
            readyQueue.push(i);
        }
    }
    // if(readyQueue.empty()){
    //     currentTime=currentTime+0.01;
    // }
}

void updateQueue(Process processes[], const int n, const float quantum, queue<int> &readyQueue, float currentTime, int programsExecuted, double avg_burstTime)
{
    // Outer loop execution until all processes are executed
    while (programsExecuted != n)
    {
        int i = readyQueue.front(); // taking the 1st arrived process

        // Dynamic RR case
        if (processes[i].burstTimeRemaining >= avg_burstTime)
        {
            // for response time calculation
            if (!processes[i].in)
            {
                processes[i].Enter = currentTime;
                processes[i].in = true;
            }


            readyQueue.pop();
            // will be pushed back in the ready Q in case when its Remaining burst time < avg_burstTime
            // or process gets completed in RR only

            // RR Case 1- process gets completed in RR only (minute possibility)
            if (processes[i].burstTimeRemaining < quantum)
            {
                // process is complete, so setting all its parameters
                processes[i].isComplete = true;
                currentTime += processes[i].burstTimeRemaining;
                processes[i].completionTime = currentTime;
                processes[i].waitingTime = processes[i].completionTime - processes[i].arrivalTime - processes[i].burstTime;
                processes[i].turnaroundTime = processes[i].waitingTime + processes[i].burstTime;
                if (processes[i].waitingTime < 0)
                    processes[i].waitingTime = 0;
                processes[i].responseTime = processes[i].Enter - processes[i].arrivalTime;

                processes[i].burstTimeRemaining = 0;
                programsExecuted++;
                processes[i].inQueue = false;

                // checking for the arrival of any new process
                if (programsExecuted != n)
                {
                    checkForNewArrivals(processes, n, currentTime, readyQueue);
                }
            }

            // RR Case 2- process dosen't get completed and is pre-empted (maximium possibility)
            else
            {
                processes[i].burstTimeRemaining -= quantum;
                currentTime += quantum;

                // checking for the arrival of any new process
                if (programsExecuted != n)
                {
                    checkForNewArrivals(processes, n, currentTime, readyQueue);
                }

                readyQueue.push(i); // pre-emption - inserting the incomplete process back into the queue
            }
        }

        // SJF case
        else
        {
            // finding the shortest Burst time process
            int shortest_job = i;                                  // to store the index of the shortest B_time process
            float shortest_time = processes[i].burstTimeRemaining; // storing its remaining B_time

            for (int i = 0; i < n; ++i) // iterating over all the processes
            {

                // conditions
                //                        SJF case                              not completed                       for finding shortest b_time           is ready to process- in ready Q
                if (processes[i].burstTimeRemaining < avg_burstTime && processes[i].burstTimeRemaining > 0 && processes[i].burstTimeRemaining < shortest_time && processes[i].inQueue)
                {
                    shortest_job = i;
                    shortest_time = processes[i].burstTimeRemaining;
                }

                // Finding shortest burst time process until we encounter a process(that has already arrived)- in ready queue and has B_time>= avgBtime(RR case)
                if (processes[i].burstTimeRemaining >= avg_burstTime && processes[i].burstTimeRemaining > 0 && processes[i].inQueue)
                {
                    break;
                }
            }
            // for response time calculation
            if (!processes[shortest_job].in)
            {
                processes[shortest_job].Enter = currentTime;
            }

            // executing SJF proccess
            // process is complete, so setting all its parameters
            processes[shortest_job].isComplete = true;
            currentTime += processes[shortest_job].burstTimeRemaining;
            processes[shortest_job].completionTime = currentTime;
            processes[shortest_job].waitingTime = processes[shortest_job].completionTime - processes[shortest_job].arrivalTime - processes[shortest_job].burstTime;
            if (processes[shortest_job].waitingTime < 0)
                processes[shortest_job].waitingTime = 0;
            processes[shortest_job].turnaroundTime = processes[shortest_job].waitingTime + processes[shortest_job].burstTime;
            processes[shortest_job].burstTimeRemaining = 0;
            processes[shortest_job].responseTime = processes[shortest_job].Enter - processes[shortest_job].arrivalTime;
            programsExecuted++;

            // calling function to remove executed process from the Ready Queue
            remove(shortest_job, readyQueue);

            processes[i].inQueue = false;

            // checking for the arrival of any new process
            if (programsExecuted != n)
            {
                checkForNewArrivals(processes, n, currentTime, readyQueue);
            }
        }
    }
}

// function that outputs the result in terms of their PID.
void output(Process processes[], const int n)
{
    double avgWaitingTime = 0;
    double avgTurntaroundTime = 0;
    double avgResponseTime = 0;
    // sort the processes array by processes.PID
    sort(processes, processes + n, [](const Process &p1, const Process &p2)
         { return p1.pid < p2.pid; });

    cout << "\n\n\t\t The Process Status \n\n";
    cout << "\tProcess ID\tArrival Time\tBurst Time\tCompletion Time\tTurn Around Time\tWaiting Time\tResponse Time";
    for (int i = 0; i < n; ++i)
    {
        cout << "\n\t\t" << processes[i].pid << "\t\t" << processes[i].arrivalTime << "\t\t" << processes[i].burstTime << "\t\t"
             << processes[i].completionTime << "\t\t" << processes[i].turnaroundTime << "\t\t" << processes[i].waitingTime << "\t\t" << processes[i].responseTime;
        avgWaitingTime += processes[i].waitingTime;
        avgTurntaroundTime += processes[i].turnaroundTime;
        avgResponseTime += processes[i].responseTime;
    }
    avgWaitingTime /= n;
    avgTurntaroundTime /= n;
    avgResponseTime /=n;
    cout << "\n\n\t\tAverage Waiting Time: " << avgWaitingTime;
    cout << "\n\t\tAverage Turn Around Time: " << avgTurntaroundTime;
    cout << "\n\t\tAverage Response Time: " << avgResponseTime;
}

// processes are already sorted according to their arrival time
void hybrid_scheduling(Process processes[], int n, float quantum, double avg_burstTime)
{
    queue<int> readyQueue;
    readyQueue.push(0); // initially, pushing the first process which arrived first
    processes[0].inQueue = true;

    float currentTime = 0;    // holds the current time after each process has been executed
    int programsExecuted = 0; // holds the number of programs executed so far - for end case

    // calling the main algorithm
    updateQueue(processes, n, quantum, readyQueue, currentTime, programsExecuted, avg_burstTime);
}

int main()
{
    int n;

    // Prompt the user to enter the number of processes
    cout << "Enter the number of processes: ";
    cin >> n;

    // Create an array of Process objects with size n + 1
    Process processes[n + 1];

    // Loop to input arrival time and burst time for each process
    for (int i = 0; i < n; i++)
    {
        // Prompt the user to enter arrival time and burst time for the current process
        cout << "Enter arrival time and burst time of each process " << i + 1 << ": ";
        cin >> processes[i].arrivalTime;
        cin >> processes[i].burstTime;

        processes[i].in = false; // for response time calc
        
        // Initialize burstTimeRemaining to the initial burst time of the process
        processes[i].burstTimeRemaining = processes[i].burstTime;

        // Assign a unique process ID to the process
        processes[i].pid = i + 1;
        cout << endl;
    }

    // Sort the processes array based on arrival time using the STL sort function
    std::sort(processes, processes + n, [](const Process &p1, const Process &p2)
              { return p1.arrivalTime < p2.arrivalTime; });

    float total_burstTime = 0;
    int num_processes = n;

    // Calculate the total burst time of all processes
    for (int i = 0; i < n; i++)
    {
        total_burstTime += processes[i].burstTime;
    }

    // Calculate the average burst time of processes
    double avg_burstTime = static_cast<double>(total_burstTime) / num_processes;

    float sum_burstTime_RR = 0;
    float num_RR_processes = 0;

    // Loop to count the number of processes with burst time greater than or equal to the average burst time
    for (int i = 0; i < n; i++)
    {
        if (processes[i].burstTime >= avg_burstTime)
        {
            sum_burstTime_RR += processes[i].burstTime;
            num_RR_processes++;
        }
    }

    float time_quantum;

    // Calculate the time quantum for the Round Robin scheduling algorithm
    if ((sum_burstTime_RR / (num_RR_processes * num_RR_processes)) > 2)
    {
        time_quantum = sum_burstTime_RR / (num_RR_processes * num_RR_processes);
    }
    else
    {
        time_quantum = 2;
    }

    // Output the average burst time and time quantum
    cout << endl
         << "Average burst time= " << avg_burstTime << endl;
    cout << "Time quantum= " << time_quantum << endl;

    // Call the hybrid_scheduling function with the sorted processes array, number of processes, time quantum, and average burst time
    hybrid_scheduling(processes, n, time_quantum, avg_burstTime);

    // Call the output function to display the scheduling results
    output(processes, n);

 return 0;
}