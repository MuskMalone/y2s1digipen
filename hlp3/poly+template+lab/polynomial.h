#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <iostream>
#include <vector>

namespace HLP3 {

template <typename T, int N>
class Polynomial {
private:
    std::vector<T> coefficients;

public:
    // Default constructor
    Polynomial();

    // Single-argument conversion constructor
    template <typename U>
    Polynomial(const Polynomial<U, N>& other);

    // Overload subscript operator for non-modifiable access
    const T& operator[](int index) const;

    // Overload subscript operator for modifiable access
    T& operator[](int index);

    // Overload multiplication operator
    template <int M>
    Polynomial<T, N + M> operator*(const Polynomial<T, M>& other) const;

    // Overload function call operator for evaluation
    T operator()(T x) const;
};



} // namespace HLP3
#include "polynomial.tpp"
#endif // POLYNOMIAL_H
