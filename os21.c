#include<stdio.h>
#include<stdlib.h>

int main(int argc, char* argv[])
{
	int m = atoi(argv[3]), n = atoi(argv[4]);//number of resources=3, number of processes=4 
	int pid[5], alloc[5][3], max[5][3], avail[3];
	int newPid, newAlloc[3];

	//argv[1] is a filename to write
	//read parameter from file input.dat
	FILE* fp = fopen(argv[1], "r");
	if (fp == NULL)
	{
		printf("scenario.txt ERROR\n");    //ERROR
		return -1;
	}
	else {
		fscanf(fp, "%d %d %d\n", &avail[0], &avail[1], &avail[2]);
		for (int i = 0; i < n; i++) {
			fscanf(fp, "%d\t%d %d %d\t %d %d %d\n", &pid[i], &alloc[i][0], &alloc[i][1], &alloc[i][2], &max[i][0], &max[i][1], &max[i][2]);
		}
		fscanf(fp, "%d\t %d %d %d", &newPid, &newAlloc[0], &newAlloc[1], &newAlloc[2]);
	}

	fclose(fp);

	fp = fopen(argv[2], "w");
	fprintf(fp, "%s\n", argv[1]);
	//banker's alogorithms
	int i, j, k, process, count = 0;
	int completed[5], safeSequence[5];
	for (i = 0; i < n; i++) {
		completed[i] = 0; //number of completed process
	}
	//To use need[][]
	int** need = malloc(sizeof(int*) * n);
	for (i = 0; i < n; i++) {
		need[i] = malloc(sizeof(int*) * m);
	}

	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++)
			need[i][j] = max[i][j] - alloc[i][j];
	}

	do
	{
		process = -1;

		for (i = 0; i < n; i++)
		{
			if (completed[i] == 0)//if not completed
			{
				process = i;
				for (j = 0; j < m; j++)
				{
					if (avail[j] < need[i][j])
					{
						process = -1;
						break;
					}
				}
			}
			if (process != -1)
				break;
		}

		if (process != -1)
		{
			safeSequence[count] = process + 1;
			count++;
			for (j = 0; j < m; j++)
			{
				avail[j] += alloc[process][j];
				alloc[process][j] = 0;
				max[process][j] = 0;
				completed[process] = 1;
			}
		}
	} while (count != n && process != -1);

	if (count == n)
	{
		fputs("current state: ", fp);
		for (i = 0; i < n; i++)
			fprintf(fp, "P%d  ", safeSequence[i]);
	}
	else
		fputs("\ncurrent state: unsafe !", fp);

	//after arriving request
	for (i = 0; i < n; i++) {
		if (newPid == pid[i]) {
			alloc[i][0] = alloc[i][0] + newAlloc[0];
			alloc[i][1] = alloc[i][1] + newAlloc[1];
			alloc[i][2] = alloc[i][2] + newAlloc[2];
		}
	}
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++)
			need[i][j] = max[i][j] - alloc[i][j];
	}

	do
	{
		process = -1;

		for (i = 0; i < n; i++)
		{
			if (completed[i] == 0)//if not completed
			{
				process = i;
				for (j = 0; j < m; j++)
				{
					if (avail[j] < need[i][j])
					{
						process = -1;
						break;
					}
				}
			}
			if (process != -1)
				break;
		}

		if (process != -1)
		{
			safeSequence[count] = process + 1;
			count++;
			for (j = 0; j < m; j++)
			{
				avail[j] += alloc[process][j];
				alloc[process][j] = 0;
				max[process][j] = 0;
				completed[process] = 1;
			}
		}
	} while (count != n && process != -1);

	if (count == n)
	{
		fputs("\nAfter arriving request: ", fp);
		for (i = 0; i < n; i++)
			fprintf(fp, "P%d  ", safeSequence[i]);
	}
	else
		fputs("\nAfter arriving request: unsafe !", fp);
	fclose(fp);
}