// C program for implementation of Simulation 
#include <stdio.h> 
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>
#include "process.h"
#include "util.h"


// Function to find the waiting time for all  
// processes
void findWaitingTimeRR(ProcessType plist[], int n,int quantum) 
{ 
  //1. Create an array *rem_bt[]* to keep track of remaining burst time of processes. This array is initially a copy of *plist[].bt* (all processes burst times)
  int rem_bt[n];
  //2. Store waiting times of processes in plist[].wt. Initialize this array as 0.
  for (int i = 0; i < n; i++){
    rem_bt[i] = plist[i].bt;
    plist[i].wt = 0;
  }

  //3. Initialize time : t = 0
  int t = 0;

  while(1){
    bool done = true;

    for(int i = 0; i < n; i++){
      //4. Keep traversing the all processes while all processes are not done. Do following for i'th process if it is not done yet.
      if (rem_bt[i] > 0){
        done = false;

        if (rem_bt[i] > quantum){
          t += quantum;
        }

      else{
        t = t + rem_bt[i];
        
        plist[i].wt = t - plist[i].bt;

        rem_bt[i] = 0; 
      }
      }
    }

    if (done == true)
      break;
  }

} 

// Function to find the waiting time for all  
// processes 
void findWaitingTimeSJF(ProcessType plist[], int n)
{ 
  int rem_bt[n];
  int complete = 0, t = 0, minm = INT_MAX;
  int shortest = 0, finish_time;
  bool check = false;

  for (int i = 0; i < n; i++){
    rem_bt[i] = plist[i].bt;
    plist[i].wt = 0;
  }
    
  //Traverse until all process gets completely executed
  while (complete != n) {

  //Find process with minimum remaining time at every single time lap.
    for (int j = 0; j < n; j++){
      if ((plist[j].art <= t) && (rem_bt[j] < minm) && rem_bt[j] > 0){
        minm = rem_bt[j];
        shortest = j;
        check = true;
      }
    }

    if (check == false){
      t++;
      continue;
    }

    //Reduce its time by 1.
    rem_bt[shortest]--;

    //new minimum
    minm = rem_bt[shortest];

    //Check if its remaining time becomes 0 
    if (minm == 0)
      minm = INT_MAX;

    //Increment the counter of process completion
    if (rem_bt[shortest] == 0){
      complete++;
      check = false;

       //Completion time of *current process = current_time +1;*
      finish_time = t + 1;

      //Calculate waiting time for each completed process. *wt[i]= Completion time - arrival_time-burst_time*
      plist[shortest].wt = finish_time - plist[shortest].bt - plist[shortest].art;

      if (rem_bt[shortest] < 0)
        rem_bt[shortest] = 0;
    }

    //Increment time lap by one.
    t++;
  }
} 

// Function to find the waiting time for all  
// processes 
void findWaitingTime(ProcessType plist[], int n)
{ 
    // waiting time for first process is 0, or the arrival time if not 
    plist[0].wt = 0 +  plist[0].art; 
  
    // calculating waiting time 
    for (int  i = 1; i < n ; i++ ) 
        plist[i].wt =  plist[i-1].bt + plist[i-1].wt; 
} 
  
// Function to calculate turn around time 
void findTurnAroundTime( ProcessType plist[], int n)
{ 
    // calculating turnaround time by adding bt[i] + wt[i] 
    for (int  i = 0; i < n ; i++) 
        plist[i].tat = plist[i].bt + plist[i].wt; 
} 
  
// Function to sort the Process acc. to priority
int my_comparer(const void *this, const void *that)
{ 
  
   
    //1. Cast this and that into (ProcessType *)
    ProcessType process1 = *(ProcessType *)this;
    ProcessType process2 = *(ProcessType *)that;
    //2. return 1 if this->pri < that->pri 
    if (process1.pri < process2.pri){
      return 1;
    }
  
    return 0;
} 

//Function to calculate average time 
void findavgTimeFCFS( ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTime(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nFCFS\n");
}

//Function to calculate average time 
void findavgTimeSJF( ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeSJF(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nSJF\n");
}

//Function to calculate average time 
void findavgTimeRR( ProcessType plist[], int n, int quantum) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeRR(plist, n, quantum); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nRR Quantum = %d\n", quantum);
}

//Function to calculate average time 
void findavgTimePriority( ProcessType plist[], int n) 
{ 
  
  //1- Sort the processes (i.e. plist[]), burst time and priority according to the priority.
  qsort(plist, n, sizeof(ProcessType), my_comparer);
  //2- Now simply apply FCFS algorithm.
  findWaitingTime(plist, n);
  findTurnAroundTime(plist, n);
  
    //Display processes along with all details 
    printf("\n*********\nPriority\n");
}

void printMetrics(ProcessType plist[], int n)
{
    int total_wt = 0, total_tat = 0; 
    float awt, att;
    
    printf("\tProcesses\tBurst time\tWaiting time\tTurn around time\n"); 
  
    // Calculate total waiting time and total turn  
    // around time 
    for (int  i=0; i<n; i++) 
    { 
        total_wt = total_wt + plist[i].wt; 
        total_tat = total_tat + plist[i].tat; 
        printf("\t%d\t\t%d\t\t%d\t\t%d\n", plist[i].pid, plist[i].bt, plist[i].wt, plist[i].tat); 
    } 
  
    awt = ((float)total_wt / (float)n);
    att = ((float)total_tat / (float)n);
    
    printf("\nAverage waiting time = %.2f", awt); 
    printf("\nAverage turn around time = %.2f\n", att); 
} 

ProcessType * initProc(char *filename, int *n) 
{
  	FILE *input_file = fopen(filename, "r");
	  if (!input_file) {
		    fprintf(stderr, "Error: Invalid filepath\n");
		    fflush(stdout);
		    exit(0);
	  }

    ProcessType *plist = parse_file(input_file, n);
  
    fclose(input_file);
  
    return plist;
}
  
// Driver code 
int main(int argc, char *argv[]) 
{ 
    int n; 
    int quantum = 2;

    ProcessType *proc_list;
  
    if (argc < 2) {
		   fprintf(stderr, "Usage: ./schedsim <input-file-path>\n");
		   fflush(stdout);
		   return 1;
	   }
    
  // FCFS
    n = 0;
    proc_list = initProc(argv[1], &n);
  
    findavgTimeFCFS(proc_list, n);
    
    printMetrics(proc_list, n);
  
  // SJF
    n = 0;
    proc_list = initProc(argv[1], &n);
   
    findavgTimeSJF(proc_list, n); 
   
    printMetrics(proc_list, n);
  
  // Priority
    n = 0; 
    proc_list = initProc(argv[1], &n);
    
    findavgTimePriority(proc_list, n); 
    
    printMetrics(proc_list, n);
    
  // RR
    n = 0;
    proc_list = initProc(argv[1], &n);
    
    findavgTimeRR(proc_list, n, quantum); 
    
    printMetrics(proc_list, n);
    
    return 0; 
} 
