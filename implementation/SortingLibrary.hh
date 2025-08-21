#ifndef SORTINGLIBRARY_HH
#define SORTINGLIBRARY_HH

#include <iostream>
#include <chrono>
#include <stdexcept>
#include <fstream>

extern int ct_swap;
extern int ct_cmp;
extern int ct_read;

extern double dur_min;
extern double dur_max;
extern double dur_avg;
extern const int RUN;

extern int read_min;
extern int read_max;
extern long read_avg;

extern int max_dim;
extern int test;
extern int ntests;
extern int details;
extern int other;

extern int n; /// dimension of array

extern std::chrono::time_point<std::chrono::high_resolution_clock> start;
extern std::chrono::time_point<std::chrono::high_resolution_clock> end;

void print_array(int *A, int dim);
void Bitsort(int*& elems, const int datasetSize, unsigned int max_freq );
void quick_sort(int *A, int p, int r);
void swap(int &a, int &b);
int partition(int *A, int p, int r);
void tim_sort(int *A, int n);
void insertion_sort(int *A, int left, int right);   //For timsort
void merge(int *A, int left, int mid, int right);   //For timsort


#endif
