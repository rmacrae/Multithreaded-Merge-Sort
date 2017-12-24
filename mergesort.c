#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX_SIZE 10000

/* Ryan MacRae
 * CPSC 3220
 * Project 2: Parallel Merge Sort
 * October 12, 2017
 * Description: This program will sort a list inputted to stdin using the
 * merge sort algorithm.  The list is sorted in parallel using threads.
 */

/*
 * List: Pointer to array for initial / temporary data
 * Sorted: Pointer to sorted array
 */
int *list;
int *sorted;
/*
 * This struct holds the parameters for the sort function.
 * This is necessary because only one argument can be passed in using
 * pthreads.
 */
struct range {
   int start;
   int end;
};
void* sort(void* arg) {
   struct range* current = (struct range*) arg;
   int start = current->start;
   int end = current->end;
   /*
    * Exit thread once start is equal to end (one element).
    */
   if(start == end)
      pthread_exit(NULL);
   /*
    * Split the array in the middle and call sort on each half.
    * Create two structs to hold the lower and upper arguments.
    */
   struct range lower;
   struct range upper;

   int middle = (start + end)/ 2;

   lower.start = start;
   lower.end = middle;

   upper.start = middle + 1;
   upper.end = end;

   pthread_t thread_low;
   pthread_t thread_high;

   printf("Creating Thread for right half of list index %d-%d\n", upper.start,
   upper.end);
   pthread_create(&thread_high, NULL, sort, (void*) &upper);

   printf("Creating Thread for left half of list index %d-%d\n", lower.start,
   lower.end);
   pthread_create(&thread_low, NULL, sort, (void*) &lower);

   printf("Exiting Thread for index %d-%d\n", upper.start, upper.end);
   pthread_join(thread_high, NULL);

   printf("Exiting Thread for index %d-%d\n", lower.start, lower.end);
   pthread_join(thread_low, NULL);

   int k = start, i = start, j = middle+1;
   /*
    * Merge the sorted values together and save the result.
    */
   printf("Merging for index %d-%d\n", k, j);
   	while (i<=middle && j<=end){
   		if (list[i] < list[j])
   			sorted[k++] = list[i++];
   		else
   			sorted[k++] = list[j++];
   	}
   	while(i<=middle)
   		sorted[k++] = list[i++];
   	while(j<=end)
   		sorted[k++] = list[j++];

   	for (i=start; i <= end;i++)
   		list[i] = sorted[i];

      return 0;
}

int main(void) {
   /*
    * Allocate space and read in integers from stdin.
    */
   int number = 0;
   list = (int*)malloc(sizeof(int)*MAX_SIZE);
   sorted = (int*)malloc(sizeof(int)*MAX_SIZE);
   while(scanf("%d", &list[number]) == 1){
      number++;
   }
   /*
    * Check for edge cases, call the sort function, print the result.
    */
   if (number <= 0) {
      printf("There are no input numbers\n");
   }
   else if (number == 1) {
      printf("The List Sorted\n");
      printf("%d\n", list[0]);
   }
   else {
      struct range beginning;
      beginning.start = 0;
      beginning.end = number -  1;
      sort(&beginning);
   }
   printf("The List Sorted:\n");
   for (int i = 0; i < number; i++) {
      printf("%d\n", sorted[i]);
   }
   return 0;
}
