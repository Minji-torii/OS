#include<stdlib.h>
#include<stdio.h>
//global variable
int np, totalBurst, arriv[10], bur[10], burCopy[10], pid[10], priority[10], firstarriv, * currentPro, * waitingTime;
float awt = 0, att;

//arrays used to draw Gantt Chart
int* pidGantt, * burGantt, ganttSize;

void calculate();
int findcurr(int);

int main(int argc, char* argv[])
{
    int i, j, temp, np = 0;
    int alpha = atoi(argv[4]);//for aging

    waitingTime = (int*)malloc(sizeof(int) * 10);
    //make size 10


    //argv[1] is a filename to write
    //read parameter from file input.dat
    FILE* fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("input.txt ERROR\n");    //ERROR
        return -1;
    }
    else {
        while (!feof(fp)) {
            fscanf(fp, "%d\t%d\t%d\t%d\n", &pid[np], &priority[np], &arriv[np], &bur[np]);
            np++;
        }
    }

    fclose(fp);


    firstarriv = 2001;

    for (i = 0; i < np; i++)
    {
        waitingTime[i] = 0;
        burCopy[i] = bur[i];
        totalBurst += bur[i];
        //find first arrive process
        if (firstarriv > arriv[i])
            firstarriv = arriv[i];
    }

    currentPro = (int*)malloc(sizeof(int) * totalBurst);

    calculate();
    //print 
    //argv[2] is a filename to write
    fp = fopen(argv[2], "w");

    fputs("Scheduling : Preemptive Priority with Aging\n", fp);
    fputs("==========================================================================\n", fp);
    fputs("each arrival time : \n", fp);
    for (i = 0; i < np; i++) {
        fprintf(fp, "P%d : %d\n", pid[i], arriv[i]);
    }

    fputs("\n**Gantt Chart**\n", fp);
    int counter, temppi, curr;

    for (i = 0; i < totalBurst + 2 + ganttSize; i++)
        fputs("-", fp);
    fputs("\n|", fp);
    counter = 0, temppi = 0;
    for (i = 0; i < totalBurst; i++)
        if (i == burGantt[counter] * temppi)
        {
            counter++;
            temppi = i;
            fputs("|", fp);
        }
        else if (i == burGantt[counter] / 2 + temppi)
            fprintf(fp, "P%d", pidGantt[counter]);
        else
            fputs(" ", fp);
    fputs("|", fp);
    fputs("\n", fp);
    for (i = 0; i < totalBurst + 2 + ganttSize; i++)
        fputs("-", fp);
    fputs("\n", fp);

    // printing the time labels
    counter = 0;
    temppi = 0;
    curr = firstarriv;
    fputs("0 ", fp);
    fprintf(fp, "%d", curr);
    for (i = 0; i < totalBurst; i++)
        if (i == burGantt[counter] + temppi)
        {
            temppi = i;
            curr += burGantt[counter];
            awt += curr;
            counter++;
            fprintf(fp, "%2d", curr);
        }
        else
        {
            fputs(" ", fp);
        }
    curr += burGantt[counter];
    fprintf(fp, "%2d\n\n", curr);

    fputs("==========================================================================\n\n", fp);
    fprintf(fp, "Average cpu usage : %.2f%%\n", (double)totalBurst / (double)(totalBurst + arriv[0]) * 100.00);
    fprintf(fp, "Average wating time : %.2f\n", awt);
    fprintf(fp, "Average response time : %.2f\n");
    fprintf(fp, "Average turnaround time : %.2f\n", att);
}

//which process running each secound
void calculate()
{
    int i, j, pNumber, prevProcess, tempbur, counter, prevProcesss;
    for (i = firstarriv; i < totalBurst + firstarriv; i++)
    {
        pNumber = findcurr(i);
        currentPro[i - firstarriv] = pNumber;
        bur[pNumber]--;
        /*claculate waiting time of each process;*/
        for (j = 0; j < np; j++) {
            if (bur[j] != 0 && arriv[j] <= i && j != pNumber)
                waitingTime[j]++;
        }
    }

    // number of context switch
    ganttSize = 1;
    prevProcess = currentPro[0];
    for (i = 0; i < totalBurst; i++)
    {
        if (prevProcess != currentPro[i])
            ganttSize++;
        prevProcess = currentPro[i];
    }

    // for Gantt Chart
    pidGantt = (int*)malloc(sizeof(int) * ganttSize);
    burGantt = (int*)malloc(sizeof(int) * ganttSize);
    prevProcess = currentPro[0];
    tempbur = 0;
    counter = 0;
    for (i = 0; i < totalBurst; i++)
    {
        if (prevProcess != currentPro[i])
        {
            pidGantt[counter] = prevProcess;
            burGantt[counter] = tempbur;
            counter++;
            tempbur = 0;
        }
        tempbur++;
        prevProcess = currentPro[i];
    }

    burGantt[ganttSize - 1] = tempbur;
    pidGantt[ganttSize - 1] = prevProcess;


    // calculate the awt & at 
    awt = 0;
    att = 0;
    for (i = 0; i < np; i++)
    {
        awt += waitingTime[i];
        att += waitingTime[i] + burCopy[i];
    }
    awt = awt / (float)np;
    att = att / (float)np;

}

//return current ruuning process pid
int findcurr(int currentTime)
{
    int i, max = 0, pidN = 0;

    //Aging
    for (i = 0; i < np; i++) {
        priority[i] = priority[i] + waitingTime[i];
    }

    for (i = 0; i < np; i++)
        if (arriv[i] <= currentTime && max < priority[i] && bur[i] != 0)
        {
            max = priority[i];
            pidN = i;
        }
    return pidN;
}