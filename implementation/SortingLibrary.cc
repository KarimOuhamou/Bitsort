#include "SortingLibrary.hh"

int ct_swap = 0;
int ct_cmp = 0;
int ct_read = 0;

double dur_min = -1;
double dur_max = -1;
double dur_avg = 0;
const int RUN = 32;

int read_min = -1;
int read_max = -1;
long read_avg = 0;

int max_dim = 1000;
int test = 0;
int ntests = 100;
int details = 0;
int other = 0;
int n = max_dim;

std::chrono::time_point<std::chrono::high_resolution_clock> start;
std::chrono::time_point<std::chrono::high_resolution_clock> end;

//This function orders in not-decreasing order positive integer numbers in range (0 ,max_value) with maximum bits_precision occurences for each value in Theta(dataSetSize + (max_value*bits_precision)/32 ) times
void Bitsort(int*& elems, const int datasetSize, unsigned int max_freq){
    int* A = new int[datasetSize];      //It will contain the sorted array

    //Preprocessing to find the maximum value in the dataset    
    int max_value = 0;
    for (int i = 0; i < datasetSize; i++) {
        ct_cmp++;
        int& value = elems[i];
        ct_read++;
        if (value > max_value) {
            ct_cmp++;
            max_value = value;
        }
    }
    max_value++; // Increment to include the max value itself in the range

    //How many bits can I rappresent the frequency of my values?
    unsigned int bits_precision =0;
    while(max_freq> 0){
        max_freq>>=1;
        bits_precision++;
    }

    //How many values can I rappresent with the bits_precision?
    unsigned int possible_values = 32/bits_precision;

    //Calculates the dim of the bitmask
    unsigned int dim = max_value/possible_values;
    if( dim % possible_values!=0 ){
        dim++;
    }

    unsigned int* bitmask = new unsigned int[dim]();

    //Creates the mask for frequencies
    unsigned int freq_mask = 0;
    for(unsigned int i=0; i< bits_precision; i++){
        freq_mask<<=1;
        freq_mask|=0b1;
    }

    //Populates the bitmask with values and relative frequencies
    for (unsigned int i = 0; i < datasetSize; i++) {
        ct_cmp++;

        int& value = elems[i];
        ct_read++;

        /*  Structure of the value in the Bitmask:     
            bitsPrecision to establish the frequency in range (0, (2^bitsPrecision)-1)
            
            Example with bitsPrecision=3:
            0b111
        */

        //Chooses the chunk which apparteins my value
        unsigned int &chunk = bitmask[value/possible_values];    
        ct_read++;

        //Decides the position of the value over the chunk
        unsigned int pos_value = (value%possible_values) * bits_precision;   

        //Increments the frequency of the value analized in the bitmask
        unsigned int freq = (chunk >> (pos_value)) & freq_mask;  
        freq++;
    
        //Saves the new frequency in the bitmask
        chunk &= ~(freq_mask << pos_value);    
        chunk |= (freq << pos_value);       
    }

    //Insertion of the values in the sorted array
    unsigned int value = 0;
    unsigned int counter = 0;
    for(unsigned int i=0; i<dim ;i++){
        ct_cmp++;
        unsigned int& chunk= bitmask[i];
        ct_read++;
        unsigned int bits_readen=0;
        //Reads the chunk and extracts the values
        while(bits_readen < possible_values * bits_precision){
            ct_cmp++;
            //Reads the frequency of the value
            unsigned int freq = (chunk) & freq_mask;   
            //Adds the value in the sorted array frequency times
            for(unsigned int j=0; j<freq; j++){
                ct_cmp++;
                A[counter] = value;
                counter++;               
            }
            //Goes to the next value
            chunk>>= bits_precision;
            value++;
            bits_readen+= bits_precision;
        }
    }

    delete[] elems;
    delete[] bitmask;
    elems = A;
    return;
}

void print_array(int *A, int dim) {
    for (int j = 0; j < dim; j++) {
        std::cout<< A[j] <<" ";
    }
    std::cout<<std::endl;
}

void swap(int &a, int &b) {
    int tmp = a;
    a = b;
    b = tmp;
    /// aggiorno contatore globale di swap
    ct_swap++;
}

int partition(int *A, int p, int r) {
    /// copia valori delle due meta p..q e q+1..r
    ct_read++;
    int x = A[r];
    int i = p - 1;

    for (int j = p; j < r; j++) {
        ct_cmp++;
        ct_read++;
        if (A[j] <= x) {
            i++;
            ct_read++;
            ct_read++;
            swap(A[i], A[j]);
        }
    }
    ct_read++;
    ct_read++;
    swap(A[i + 1], A[r]);

    return i + 1;
}


void quick_sort(int *A, int p, int r) {
    /// gli array L e R sono utilizzati come appoggio per copiare i valori: evita le allocazioni nella fase di merge
    if (p < r) {
        int q = partition(A, p, r);
        quick_sort(A, p, q - 1);
        quick_sort(A, q + 1, r);
    }
}

// Funzione per ordinare piccoli blocchi con Insertion Sort
void insertion_sort(int *A, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int temp = A[i];
        ct_read++; // Lettura di A[i]
        int j = i - 1;
        while (j >= left) {
            ct_cmp++; // Confronto A[j] > temp
            ct_read++; // Lettura di A[j]
            if (A[j] > temp) {
                A[j + 1] = A[j];
                ct_read++; // Lettura di A[j] per assegnazione
                j--;
            } else {
                break;
            }
        }
        A[j + 1] = temp;
        // Non serve ct_read qui: temp era già letto
    }
}

// Funzione per fondere due sotto-array ordinati
void merge(int *A, int left, int mid, int right) {
    int len1 = mid - left + 1, len2 = right - mid;
    int *L = new int[len1];
    int *R = new int[len2];

    for (int i = 0; i < len1; i++) {
        L[i] = A[left + i];
        ct_read++; // Lettura di A[left + i]
    }
    for (int i = 0; i < len2; i++) {
        R[i] = A[mid + 1 + i];
        ct_read++; // Lettura di A[mid + 1 + i]
    }

    int i = 0, j = 0, k = left;
    while (i < len1 && j < len2) {
        ct_cmp++; // Confronto L[i] <= R[j]
        ct_read++; // Lettura di L[i]
        ct_read++; // Lettura di R[j]
        if (L[i] <= R[j]) {
            A[k++] = L[i++];
            // ct_read già contato sopra per L[i]
        } else {
            A[k++] = R[j++];
            // ct_read già contato sopra per R[j]
        }
    }

    while (i < len1) {
        A[k++] = L[i++];
        ct_read++; // Lettura di L[i]
    }

    while (j < len2) {
        A[k++] = R[j++];
        ct_read++; // Lettura di R[j]
    }

    delete[] L;
    delete[] R;
}

// Funzione principale di Timsort
void tim_sort(int *A, int n) {
    // Ordina piccoli blocchi con Insertion Sort
    for (int i = 0; i < n; i += RUN) {
        insertion_sort(A, i, std::min(i + RUN - 1, n - 1));
    }

    // Fonde i blocchi ordinati
    for (int size = RUN; size < n; size = 2 * size) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = left + size - 1;
            int right = std::min(left + 2 * size - 1, n - 1);

            if (mid < right) {
                merge(A, left, mid, right);
            }
        }
    }
}
