#include <stdio.h>
#include "Simulation.h"

int main()
{
    pcb_t processControlBlock[10];
    for (int i = 0; i < 5; i++)
    {
        processControlBlock[i].allocate_pcb(); //Create a new processes   
        //hover +=processControlBlock[i].burstTime;//Calculate the total time required to finish all processes
    }

     for(int i=0; i<5; i++)
        {
            if(processControlBlock[i] != NULL)
            {
               // plist[i].updateProcessBlock("Ready");//Load the PCB ofprocess number i
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