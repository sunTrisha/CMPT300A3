#include <stdio.h>
int main()
{
    int hover=0;
    int timeSlot=1; //Set processor time slice
    ProccessBlockList plist[10];//Declare PCB for  processes
    for(int k=0;k<5;k++)
    {
        plist[k].addProcess((k+1)); //Create a new processes   
        hover +=plist[k].burstTime;//Calculate the total time required to finish all processes
    }
    //start process execution
    while(hover != 0)
    {
        for(int i=0; i<5; i++)
        {
            if(plist[i].burstTime > 0)
            {
                if(plist[i].burstTime > timeSlot)
                {
                    plist[i].updateProcessBlock("Ready");//Load the PCB ofprocess number i
                    plist[i].executeProcess(timeSlot); //Run the Process number i
                    plist[i].burstTime -= timeSlot; //Reduce the burst time by time slot
                    hover -= timeSlot; //Reduce the total time required to finish all processes
                }
                else
                {
                    plist[i].updateProcessBlock("Ready"); //Load and update the PCB of process number i
                    plist[i].executeProcess(plist[i].burstTime);//Run the Process number i
                    hover -= plist[i].burstTime; //Reduce the total time required to finish all processes
                    plist[i].burstTime = 0; //Set Burst Time to zero if finished
                }
            }
        }
    }
}