#ifndef MPITYPE
#define MPITYPE
#include <mpi.h>


template <typename T>
MPI_Datatype getMPIType() {
    if (std::is_same<T, int>::value) {
        return MPI_INT;
    } 
    else if (std::is_same<T, float>::value) {
        return MPI_FLOAT;
    } 
    else if (std::is_same<T, double>::value) {
        return MPI_DOUBLE;
    }
    else if (std::is_same<T, char>::value) {
        return MPI_CHAR;
    }
    else if (std::is_same<T, long>::value) {
        return MPI_LONG;
    }
    else if (std::is_same<T, long long>::value) {
        return MPI_LONG_LONG;
    }
    else if (std::is_same<T, short>::value) {
        return MPI_SHORT;
    }
    else if (std::is_same<T, unsigned>::value) {
        return MPI_UNSIGNED;
    }
    else if (std::is_same<T, unsigned char>::value) {
        return MPI_UNSIGNED_CHAR;
    }
    else if (std::is_same<T, unsigned long>::value) {
        return MPI_UNSIGNED_LONG;
    }
    else if (std::is_same<T, unsigned long long>::value) {
        return MPI_UNSIGNED_LONG_LONG;
    }
    else if (std::is_same<T, unsigned short>::value) {
        return MPI_UNSIGNED_SHORT;
    }
    else if (std::is_same<T, bool>::value) {
        return MPI_C_BOOL;
    }
    else if (std::is_same<T, std::complex<float>>::value) {
        return MPI_C_FLOAT_COMPLEX;
    }
    else if (std::is_same<T, std::complex<double>>::value) {
        return MPI_C_DOUBLE_COMPLEX;
    }
    else if (std::is_same<T, std::complex<long double>>::value) {
        return MPI_C_LONG_DOUBLE_COMPLEX;
    }
    else if (std::is_same<T, std::pair<int, int>>::value) {
        return MPI_2INT;
    }
    else if (std::is_same<T, std::pair<float, int>>::value) {
        return MPI_FLOAT_INT;
    }
    else if (std::is_same<T, std::pair<double, int>>::value) {
        return MPI_DOUBLE_INT;
    }
    else if (std::is_same<T, std::pair<long, int>>::value) {
        return MPI_LONG_INT;
    }
    else if (std::is_same<T, std::pair<short, int>>::value) {
        return MPI_SHORT_INT;
    }
    else if (std::is_same<T, std::pair<unsigned, int>>::value) {
        return MPI_UNSIGNED;
    }
    else if (std::is_same<T, std::pair<unsigned char, int>>::value) {
        return MPI_UNSIGNED_CHAR;
    }
    else if (std::is_same<T, std::pair<unsigned long, int>>::value) {
        return MPI_UNSIGNED_LONG;
    }
    else if (std::is_same<T, std::pair<unsigned long long, int>>::value) {
        return MPI_UNSIGNED_LONG_LONG;
    }
    else if (std::is_same<T, std::pair<unsigned short, int>>::value) {
        return MPI_UNSIGNED_SHORT;
    }
    else if (std::is_same<T, std::pair<bool, int>>::value) {
        return MPI_C_BOOL;
    }
    else if (std::is_same<T, std::pair<std::complex<float>, int>>::value) {
        return MPI_C_FLOAT_COMPLEX;
    }
    else if (std::is_same<T, std::pair<std::complex<double>, int>>::value) {
        return MPI_C_DOUBLE_COMPLEX;
    }
    else if (std::is_same<T, std::pair<std::complex<long double>, int>>::value) {
        return MPI_C_LONG_DOUBLE_COMPLEX;
    }
    else if (std::is_same<T, std::string>::value) {
        return MPI_CHAR;
    }
    else if (std::is_same<T, void>::value) {
        return MPI_BYTE;
    }
    else {
        return MPI_DATATYPE_NULL;
    }
}
    
#endif // MPITYPE