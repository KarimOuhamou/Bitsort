#include "SortingLibrary.hh"

//  AUTHORS:    KARIM OUHAMOU, GIUSEPPE DIDONNA

// Compilation: g++ -O2 ./Bitsort.cc ./SortingLibrary.cc -o ./Bitsort.out
// Requires a file named data.txt in the same directory containing the datasets.

// The program reads the data.txt file, which contains 100 rows of data to sort.
// Usage:
//   ./Bitsort.out        -> run the main Bitsort algorithm
//   ./Bitsort.out -o     -> run the alternative algorithm for comparison
//   -v flag              -> enable verbose mode
//
// Output:
//   For each row, the program shows the number of memory accesses and the execution time.

// Objective:
//   Implement a sorting algorithm that minimizes memory accesses.

void parse_cmd(int argc, char **argv) {
    /// parsing argomento
    for (int i = 1; i < argc; i++) {
        if (argv[i][1] == 't')
            ntests = atoi(argv[i] + 3);
        if (argv[i][1] == 'v')
            details = 1;
        if(argv[i][1]=='o'){
            other = 1;
        }
    }
}

void update_statistics(){
    std::chrono::duration<double, std::milli> duration = end - start;
    dur_avg += duration.count();
    if (dur_min < 0 || dur_min > duration.count())
    dur_min = duration.count();
    if (dur_max < 0 || dur_max < duration.count())
    dur_max = duration.count();

    read_avg += ct_read;
    if (read_min < 0 || read_min > ct_read)
    read_min = ct_read;
    if (read_max < 0 || read_max < ct_read)
    read_max = ct_read;
}

//Checks if the array is sorted
bool checkFailure(int*& arrayToCheck){
    for (int i = 0; i < n-1; i++) {
        if (arrayToCheck[i] > arrayToCheck[i+1]) {
            std::cout << arrayToCheck[i] << " > " << arrayToCheck[i+1] << std::endl;    
            return true;
        }
    }
    return false;
}

//Reads the data from a stream and populates the array
void populate_Array(int*& elems, std::ifstream& input_data){
    //Popolo array con i valori del dataset
    for (int i = 0; i < n; i++) {
        char comma;
        input_data >> elems[i];
        input_data >> comma;
    }
    return ;
}

//Tests the sorting algorithms
void testAlgos(int*& A, std::ifstream& input_data){
    start = std::chrono::high_resolution_clock::now();

    if(other){
        //quick_sort(A, 0, n-1);         //Execution of the secondary algo
        tim_sort(A, n);
    }
    else{
        Bitsort(A, n, 5);               //Execution of the main algo
    }
    
    end = std::chrono::high_resolution_clock::now();

    if(details){
        std::cout << "Output" << std::endl;
        print_array(A, n);
    }
    return ;
}

//Main function that runs the tests 
void tester(){
    int* A;
    std::ifstream input_data;
    input_data.open("data.txt");
    if (!input_data) {
        std::cerr << "Errore apertura file data.txt" << std::endl;
        return;
    }
    A = new int[max_dim];

    for (test = 0; test < ntests; test++) {
        input_data.clear();
        input_data.seekg(0, std::ios::beg);
        ct_swap = 0;
        ct_cmp = 0;
        ct_read = 0;

        populate_Array(A, input_data);
        testAlgos(A, input_data);
        //Check if the array is sorted
        if(checkFailure(A)){
            input_data.close();
            delete[] A;
            A = nullptr;
            throw std::runtime_error("Array not sorted");
        }
        
        //Calculate statistics
        update_statistics();

        std::cout << "Test " << test << ": " << ct_read << std::endl;
    }
    //Print statistics
    std::cout << "N test: " << ntests << ", Min: " << read_min << ", Med: " << (0.0 + read_avg) / ntests << ", Max: "<< read_max << std::endl;
    std::cout << "Durate: Min: " << dur_min << ", Med: " << dur_avg/ntests << ", Max: "<<dur_max<<std::endl;
    std::cout << "Swap: " << ct_swap << ", Cmp: " << ct_cmp << ", Read: " << ct_read << std::endl;
    
    //Deallocating memory
    delete[] A;
    A = nullptr;
    //Close the input file
    input_data.close();
}


int main(int argc, char **argv) {
    parse_cmd(argc, argv);

    tester();

    return 0;
}
