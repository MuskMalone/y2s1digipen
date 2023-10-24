// Don't forget file-header and function-level Doxygen documentation blocks!!!

// First, define the class template. Then, define the member functions of the class template
// OUTSIDE the class definition. The automatic grader will not be able to check for this - 
// however, the manual grader can. Occurrences of EACH such function defined in the class definition
//  will result in a deduction of ONE LETTER GRADE!!! You're forewarned!!!

// This is how you should implement the assignment:

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
        class RowProxy{
            pointer r;
            public: 
                RowProxy(pointer r);
                reference operator[](size_t i);
        };

        using row = RowProxy;
        using col = reference;
    private:
        size_type rows;
        size_type cols;
        pointer data;

    public:
        Matrix(size_type nr, size_type nc);
        Matrix(Matrix const& rhs);
        Matrix(Matrix&& rhs) noexcept;
        Matrix(std::initializer_list<std::initializer_list<value_type>> m);
        ~Matrix() noexcept;
        Matrix& operator=(Matrix&& rhs);
        Matrix& operator=(Matrix const& rhs);
        size_type get_rows() const noexcept;
        size_type get_cols() const noexcept;
        row operator[](size_type index) const;
    };
    template <typename T>
    Matrix<T>::RowProxy::RowProxy(pointer r) : r{r}{
        
    }
    template <typename T>
    Matrix<T>::reference  Matrix<T>::RowProxy::operator[](size_t i){
        return r[i];
    }
    template<typename T>
    Matrix<T>::Matrix<T>(size_type nr, size_type nc) : rows{nr}, cols{nc}, data{new value_type[nr * nc]{}}{}
    
    template<typename T>
    Matrix<T>::Matrix<T>(Matrix<T> const& rhs) : rows{rhs.rows}, cols{rhs.cols}, data{new value_type[rhs.rows * rhs.cols]{}}{
        for (int i{}; i < rows; ++i){
            for (int j{}; i < cols; ++j){
                data[i * cols + j] = rhs.data[i * cols + j];
            }
        }
    }

    template <typename T>
    Matrix<T>::Matrix<T>(Matrix<T>&& rhs) noexcept {
        rows = std::move(rhs.rows);
        cols = std::move(rhs.cols);  
        data = nullptr; 
        std::swap(data, rhs.data);
    }
    
    template <typename T>
    Matrix<T>::Matrix<T>(std::initializer_list<std::initializer_list<value_type>> m) : rows{m.size()}, cols{m.begin()->size()}, data{new value_type[m.size() * m.begin()->size()]{}}{
        size_type s {m.begin()->size()};
        size_type index{};
        for (auto const& valueList : m){
            if (valueList.size() != s) throw std::runtime_error{"bad initializer list"};
            for( auto const& value : valueList){
                *(data + index) = value;
                index++;
            }
        }
    }
    template <typename T>
    Matrix<T>::~Matrix() noexcept{ 
        if (data) delete [] data;
    }

    template <typename T>
    Matrix<T>& Matrix<T>::operator=(Matrix<T>&& rhs) noexcept{
        std::swap(rows, rhs.rows);
        std::swap(cols, rhs.cols);
        std::swap(data, rhs.data);
        return *this;
    }

    template <typename T>
    Matrix<T>& Matrix<T>::operator=(Matrix<T> const& rhs){
        Matrix c {rhs};
        std::swap(*this, c);
        return *this;
    }

    template <typename T>
    Matrix<T>::size_type Matrix<T>::get_rows() const noexcept { 
        return rows;
    }

    template <typename T>
    Matrix<T>::size_type Matrix<T>::get_cols() const noexcept { 
        return cols; 
    }

    template <typename T>
    Matrix<T>::row Matrix<T>::operator[](size_type index) const{
        return RowProxy(data + (index * cols));
    }
    // declare global functions for following operator overloads:
    // 1. +: adding two Matrix<T> objects
    template <typename T>
    Matrix<T> operator+(Matrix<T> const& a, Matrix<T> const& b){
        if (a.get_cols() != b.get_cols() || a.get_rows() != b.get_rows()) throw std::runtime_error{"operands for matrix addition must have same dimensions"};
        Matrix<T> temp {a};
        for (size_t rows{}; rows < a.get_rows(); ++rows){
            for (size_t cols{}; cols < a.get_cols(); ++cols){
                temp[rows][cols] += b[rows][cols];
            }
        }
        return temp;
    }
    // 2. -: subtracting two Matrix<T> objects
    template <typename T>
    Matrix<T> operator-(Matrix<T> const& a, Matrix<T> const& b){
        if (a.get_cols() != b.get_cols() || a.get_rows() != b.get_rows()) throw std::runtime_error{"operands for matrix subtraction must have same dimensions"};
        Matrix<T> temp {a};
        for (size_t rows{}; rows < a.get_rows(); ++rows){
            for (size_t cols{}; cols < a.get_cols(); ++cols){
                temp[rows][cols] -= b[rows][cols];
            }
        }    
        return temp;
    }
    // 3. *: multiplying two Matrix<T> objects
    template <typename T>
    Matrix<T> operator*(Matrix<T> const& a, Matrix<T> const& b){
        if (a.get_cols() != b.get_rows()) throw std::runtime_error{"number of columns in left operand must match number of rows in right operand"};
        Matrix<T> temp {Matrix<T>{a.get_rows(), b.get_cols()}};
        for (size_t rows{}; rows < a.get_rows(); ++rows){
            for (size_t cols{}; cols < b.get_cols(); ++cols){
                for (size_t i{}; i < a.get_cols(); ++i){
                    temp[rows][cols] += a[rows][i] * b[i][cols];
                }
            }
        }  
        return temp;
    }
    template <typename T, typename U>
    Matrix<T> operator*(U scalar, Matrix<T> const& b){
        Matrix<T> temp {b};
        for (size_t rows{}; rows < b.get_rows(); ++rows){
            for (size_t cols{}; cols < b.get_cols(); ++cols){
                temp[rows][cols] *= scalar;
            }
        }  
        return temp;
    }
    template <typename T, typename U>
    Matrix<T> operator*(Matrix<T> const& b, U scalar){
        Matrix<T> temp {b};
        for (size_t rows{}; rows < b.get_rows(); ++rows){
            for (size_t cols{}; cols < b.get_cols(); ++cols){
                temp[rows][cols] *= scalar;
            }
        }  
        return temp;
    }
    // 4. ==: compare two Matrix<T> objects for equality
    template <typename T>
    bool operator==(Matrix<T> const& a, Matrix<T> const& b){
        if (a.get_cols() != b.get_cols() || a.get_rows() != b.get_rows()) return false;
        for (size_t rows{}; rows < a.get_rows(); ++rows){
            for (size_t cols{}; cols < a.get_cols(); ++cols){
                if (a[rows][cols] != b[rows][cols]) return false;
            }
        }    
        return true;
    }
    // 5. !=: compare two Matrix<T> objects for inequality
    template <typename T>
    bool operator!=(Matrix<T> const& a, Matrix<T> const& b){
        if (a.get_cols() != b.get_cols() || a.get_rows() != b.get_rows()) return true;
        size_type counter{}
        for (size_t rows{}; rows < a.get_rows(); ++rows){
            for (size_t cols{}; cols < a.get_cols(); ++cols){
                if (a[rows][cols] != b[rows][cols]) return true;
            }
        }
        return false;
    }

}

#endif
