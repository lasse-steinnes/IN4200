// programme for set 1 - exercise 3
#include "set1ex3.hpp"
#include <iostream>

//using namespace std;

void Exercise_3::swap(int *a, int *b){
       int t =*a; *a=*b; *b=t;
}

int * Exercise_3::quicksort(int *arr,int beg, int end){ // pivots around beginning index
      if (end > beg + 1) {
        int piv = arr[beg], l = beg + 1, r = end;
        while (l < r) {
          if (arr[l] <= piv)
            l++;
          else
            swap(&arr[l], &arr[--r]);
        }
        swap(&arr[--l], &arr[beg]);
        quicksort(arr, beg, l);
        quicksort(arr, r, end);
      }
      return arr;
}

int * Exercise_3::quicksort_revised(int *perm, int *arr,int beg, int end){
 /* Quicksort algo returning permutation vector to arr */
 if (end > beg + 1) {
   int piv = arr[perm[beg]], l = beg + 1, r = end;
   while (l < r) {
     if (arr[perm[l]] <= piv)
       l++;
     else
       swap(&perm[l], &perm[--r]);
   }
   swap(&perm[--l], &perm[beg]);
   quicksort_revised(perm, arr, beg, l);
   quicksort_revised(perm, arr, r, end);
 }
 return perm;
}
