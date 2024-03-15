#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Function Prototypes
void merge(int pData[], int left, int middle, int right);

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	if (l < r) {
		int middle = (l + r)/2;

		mergeSort(pData, l, middle);
		mergeSort(pData, middle + 1, r);

		merge(pData, l, middle, r);
	}
}

//NEEDED TO CREATE A MERGE FUNCTION
void merge(int pData[], int left, int middle, int right) {
	//variables needed for indexes and size
	int i, j, k;
	int n1 = middle - left + 1;
	int n2 = right - middle;

	//allocate memory for subarrays
	int* pLeft = Alloc(sizeof(int) * n1);
	int* pRight = Alloc(sizeof(int) * n2);

	//copy data into subarrays
	for (i = 0; i < n1; ++i) {
		pLeft[i] = pData[left + i];
	}
	for (j = 0; j < n2; ++j) {
		pRight[j] = pData[middle + 1 + j];
	}

	//merge temp arrays back into original array
	//initial indexes below
	i = 0; //left subarray
	j = 0; //right subarray
	k = left; //merged subarray
	while (i < n1 && j < n2) {
		if (pLeft[i] <= pRight[j]) {
			pData[k] = pLeft[i];
			++i;
		}
		else {
			pData[k] = pRight[j];
			++j;
		}
		++k;
	}

	//copy any remaining elements of left subarray
	while (i < n1) {
		pData[k] = pLeft[i];
		++i;
		++k;
	}

	//copy any remaining elements of right subarray
	while (j < n2) {
		pData[k] = pRight[j];
		++j;
		++k;
	}

	//free subarray memory
	DeAlloc(pLeft);
	DeAlloc(pRight);
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}