// Provide file documentation header
// Don't include any library headers!!!

namespace HLP3 {
// Include guards are not necessary in .tpp files since they are included only once from the corresponding header file.

template <typename T, int N>
Polynomial<T, N>::Polynomial() : coefficients(N + 1, 0) {}

template <typename T, int N>
template <typename U>
Polynomial<T, N>::Polynomial(const Polynomial<U, N>& other) {
    for (int i = 0; i <= N; ++i) {
        coefficients.push_back(static_cast<T>(other[i]));
    }
}

template <typename T, int N>
const T& Polynomial<T, N>::operator[](int index) const {
    return coefficients[index];
}

template <typename T, int N>
T& Polynomial<T, N>::operator[](int index) {
    return coefficients[index];
}

template <typename T, int N>
template <int M>
Polynomial<T, N + M> Polynomial<T, N>::operator*(const Polynomial<T, M>& other) const {
    Polynomial<T, N + M> result;
    for (int i = 0; i <= N; ++i) {
        for (int j = 0; j <= M; ++j) {
            result[i + j] += (*this)[i] * other[j];
        }
    }
    return result;
}

template <typename T, int N>
T Polynomial<T, N>::operator()(T x) const {
    T result = 0;
    T term = 1;
    for (int i = 0; i <= N; ++i) {
        result += coefficients[i] * term;
        term *= x;
    }
    return result;
}
// Define member functions of class template Polynomial ...

// DON'T CHANGE/EDIT THE FOLLOWING DEFINITION:
template< typename T, int N > 
std::ostream& operator<<(std::ostream &out, Polynomial<T, N> const& pol) {
  out << pol[0] << " ";
  for ( int i=1; i<=N; ++i ) {
    if ( pol[i] != 0 ) { // skip terms with zero coefficients
      if      ( pol[i] > 0 ) {  out << "+"; }
      if      ( pol[i] == 1 )  { }
      else if ( pol[i] == -1 ) { out << "-"; }
      else                     { out << pol[i] << "*"; }
      out << "x^" << i << " ";
    }
  }
  return out;
}

} // end namespace HLP3
