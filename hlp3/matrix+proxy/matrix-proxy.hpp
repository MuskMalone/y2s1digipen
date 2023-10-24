/**
 * @file matrix-proxy.hpp
 * @author tan cheng hian (t.chenghian@digipen.edu)
 * @brief template implementation
 * @version 0.1
 * @date 2023-10-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef MATRIX_PROXY_HPP
#define MATRIX_PROXY_HPP

#include <memory>
#include <initializer_list>
#include <exception>
#include <algorithm>
#include <utility>
// include necessary headers ...

namespace HLP3 {
    template <typename T>
    class Matrix {
    public:
    // provide common standard library container type definitions
    // with using keyword ...
        using size_type = size_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        /**
         * @brief Proxy to interact with the underlying matrix data
         */
        
        class Proxy{
            private: 
                pointer array;
            public:
                /**
                 * @brief Construct a new Proxy object
                 * 
                 * @param a Pointer to the underlying matrix data
                 */
                Proxy(pointer a) : array{a}{}
                /**
                 * @brief Overloads the subscript operator
                 * 
                 * @param index Matrix row index
                 * @return reference Reference to matrix element at given index
                 */
                
                reference operator[](size_type index){ return *(array + index); }
        };
        using row = Proxy;
    private:
        size_type rows;
        size_type cols;
        pointer data;

    public:
        /**
         * @brief Construct a new Matrix:: Matrix object
         * 
         * @param nr Number of rows
         * @param nc Number of columns
         */
        
        Matrix(size_type nr, size_type nc) : rows{nr}, cols{nc}, data{new value_type[nr * nc]{}}{}
        /**
         * @brief Move constructor
         * 
         * @param rhs Source Matrix from which to move
         */
        
        Matrix(Matrix const& rhs) : Matrix{rhs.rows, rhs.cols}{ std::copy(rhs.data, rhs.data + (rhs.rows * rhs.cols), data); }
        /**
         * @brief Move constructor
         * 
         * @param rhs Source Matrix from which to move
         */
        
        Matrix(Matrix&& rhs) noexcept : rows {std::move(rhs.rows)}, cols{std::move(rhs.cols)}, data{} { std::swap(data, rhs.data); }
        /**
         * @brief Initializes Matrix elements using a list of initializer lists
         * 
         * @param m List of initializer lists corresponding to Matrix elements
         */
        
        Matrix(std::initializer_list<std::initializer_list<value_type>> m) : Matrix{m.size(), m.begin()->size()}{
            size_type size {m.begin()->size()};
            size_type idx{};
            for (auto const& il : m){
                if (il.size() != size) throw std::runtime_error{"bad initializer list"};
                for( auto const& v : il){
                    *(data + idx) = v;
                    idx++;
                }
            }
        }
        /**
         * @brief Move assignment operator
         * 
         * @param rhs Source Matrix from which to move
         * @return Matrix& Reference to the current Matrix after moving
         */
        
        ~Matrix() noexcept{ if (data) delete [] data; }
        /**
         * @brief Copy assignment operator
         * 
         * @param rhs Source Matrix from which to copy
         * @return Matrix& Reference to the current Matrix after copying
         */
        
        Matrix& operator=(Matrix&& rhs) noexcept{
            std::swap(rows, rhs.rows);
            std::swap(cols, rhs.cols);
            std::swap(data, rhs.data);
            return *this;
        }

        /**
         * @brief Returns number of rows in the Matrix
         * 
         * @return size_type Number of rows
         */
        Matrix& operator=(Matrix const& rhs){
            Matrix temp {rhs};
            std::swap(*this, temp);
            return *this;
        }
        /**
         * @brief Returns number of columns in the Matrix
         * 
         * @return size_type Number of columns
         */
        
        size_type get_rows() const noexcept { return rows;}
        /**
         * @brief Overloads the subscript operator for Matrix
         * 
         * @param index Index of the row to access
         * @return row A Proxy object for interacting with the specified row
         */
        
        size_type get_cols() const noexcept { return cols; }
        /**
         * @brief Adds two given matrices
         * Both matrices must have same dimensions
         * 
         * @param a First matrix
         * @param b Second matrix
         * @return Matrix<T> Resultant matrix after addition
         * @throws std::runtime_error If the dimensions of both matrices don't match
         */
        row operator[](size_type index) const{ return row(data + (index * cols)); }

    };
    /**
     * @brief Adds two given matrices
     * Both matrices must have same dimensions
     * 
     * @param a First matrix
     * @param b Second matrix
     * @return Matrix<T> Resultant matrix after addition
     * @throws std::runtime_error If the dimensions of both matrices don't match
     */
    
    template <typename T>
    Matrix<T> operator+(Matrix<T> const& a, Matrix<T> const& b){
        if (a.get_cols() != b.get_cols() || a.get_rows() != b.get_rows()) throw std::runtime_error{"operands for matrix addition must have same dimensions"};
        Matrix<T> temp {a};
        for (size_t rows{}; rows < a.get_rows(); ++rows)
            for (size_t cols{}; cols < a.get_cols(); ++cols)
                temp[rows][cols] += b[rows][cols];
        return temp;
    }
    /**
     * @brief Subtracts matrix b from a
     * 
     * @param a The minuend matrix
     * @param b The subtrahend matrix
     * @return Matrix<T> The difference of two matrices
     * @throws std::runtime_error If the dimensions of both matrices don't match
     */
    
    template <typename T>
    Matrix<T> operator-(Matrix<T> const& a, Matrix<T> const& b){
        if (a.get_cols() != b.get_cols() || a.get_rows() != b.get_rows()) throw std::runtime_error{"operands for matrix subtraction must have same dimensions"};
        Matrix<T> temp {a};
        for (size_t rows{}; rows < a.get_rows(); ++rows)
            for (size_t cols{}; cols < a.get_cols(); ++cols)
                temp[rows][cols] -= b[rows][cols]; 
        return temp;
    }
    /**
     * @brief Multiplies matrix a by matrix b
     * 
     * @param a The multiplicand matrix
     * @param b The multiplier matrix
     * @return Matrix<T> The product of two matrices
     * @throws std::runtime_error If number of columns in left operand is not equal to number of rows in right operand
     */
    
    template <typename T>
    Matrix<T> operator*(Matrix<T> const& a, Matrix<T> const& b){
        if (a.get_cols() != b.get_rows()) throw std::runtime_error{"number of columns in left operand must match number of rows in right operand"};
        Matrix<T> temp {Matrix<T>{a.get_rows(), b.get_cols()}};
        for (size_t rows{}; rows < a.get_rows(); ++rows)
            for (size_t cols{}; cols < b.get_cols(); ++cols)
                for (size_t i{}; i < a.get_cols(); ++i)
                    temp[rows][cols] += a[rows][i] * b[i][cols];
        return temp;
    }
    /**
     * @brief Multiplies a matrix with a scalar value
     * 
     * @param b The matrix to be multiplied with scalar
     * @param scalar The scalar value to be multiplied
     * @return Matrix<T> Resultant matrix after multiplication
     */

    template <typename T, typename U>
    Matrix<T> operator*(U scalar, Matrix<T> const& b){
        Matrix<T> temp {b};
        for (size_t rows{}; rows < b.get_rows(); ++rows)
            for (size_t cols{}; cols < b.get_cols(); ++cols)
                temp[rows][cols] *= scalar;
        return temp;
    }
    
    template <typename T, typename U>
    Matrix<T> operator*(Matrix<T> const& b, U scalar){
        Matrix<T> temp {b};
        for (size_t rows{}; rows < b.get_rows(); ++rows)
            for (size_t cols{}; cols < b.get_cols(); ++cols)
                temp[rows][cols] *= scalar;
        return temp;
    }
    /**
     * @brief Checks if matrix a and b are not equal
     * 
     * @param a First matrix for inequality check
     * @param b Second matrix for inequality check
     * @return true If matrices a and b are not equal
     * @return false If matrices a and b are equal
     */
    
    template <typename T>
    bool operator==(Matrix<T> const& a, Matrix<T> const& b){
        if (a.get_cols() != b.get_cols() || a.get_rows() != b.get_rows()) return false;
        for (size_t rows{}; rows < a.get_rows(); ++rows)
            for (size_t cols{}; cols < a.get_cols(); ++cols)
                if (a[rows][cols] != b[rows][cols]) return false; 
        return true;
    }
    /**
     * @brief Checks if matrix a and b are not equal
     * 
     * @param a First matrix for inequality check
     * @param b Second matrix for inequality check
     * @return true If matrices a and b are not equal
     * @return false If matrices a and b are equal
     */
    
    template <typename T>
    bool operator!=(Matrix<T> const& a, Matrix<T> const& b){ return !(a==b); }
}

#endif
