// #ifndef VECTOR_OPERATIONS_H
// #define VECTOR_OPERATIONS_H
// #include <vector>
// #include <iostream>
// using std::vector;
// using std::ostream;

// template <typename T>
// ostream & operator << (ostream& out, const vector<T> & vec) {
// 	for (size_t i = 0; i < vec.size() ; ++i) {
// 			out << vec[i] << " ";
// 	}
// 	return out;
// };


// template <typename T>
// vector<T> SummedVector(const vector<T> & InitialVector) {
//     vector<T> Summed(InitialVector.size(), 0);
//     if(Summed.size() == 0) {
//         return Summed;
//     };
//     Summed[0] = InitialVector[0];
//     for(size_t i = 0; i + 1 < Summed.size(); ++i) {
//         Summed[i+1] = Summed[i] + InitialVector[i+1];
//     }
//     return Summed;
// };

// template <typename T>
// vector<T> NormalizedVector(const vector<T> & InitialVector) {
//     vector<T> Normalized(InitialVector.size(), 0);
//     if(Normalized.size() == 0) {
//         return Normalized;
//     };
//     T sum = 0;
//     for(size_t i = 0; i < Normalized.size(); ++i) {
//         sum += InitialVector[i];
//     }
//     for(size_t i = 0; i < Normalized.size(); ++i) {
//         Normalized[i] += InitialVector[i] / sum;
//     }
//     return Normalized;
// };

// template <typename T>
// size_t BinaryMinGreaterSearch(const std::vector<T> & IncreasingVector, T rnd) {
//     size_t left = 0, right = IncreasingVector.size() - 1; //binarySearch
//     size_t center;
//     while(right > left) {
//         center = (left + right) / 2;
//         if(IncreasingVector[center] > rnd) {
//             right = center;
//         } else {
//             left = center + 1;
//         }
//     }
//     return left;
// }

// #endif
