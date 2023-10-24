// Provide file documentation header
// Don't include any C and C++ standard library headers!!!
// Remember, this file is incomplete and you must provide missing details/features.

#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <iostream> // std::ostream
#include <array>
#include <cmath>                                                 

namespace HLP3 {
    template <typename T, typename U>
    T power(T base, U pow){
        return 
            static_cast<T>(
                std::pow(static_cast<double>(base), static_cast<double>(pow))
            );
    }
    template <typename T, int degree>
    class Polynomial{
        public:
            using valuetype = T;
            using valueref = T&;
        public:
            Polynomial() : coeffs{} {

            }

            //copy ctor
            template<typename Type>
            Polynomial(Polynomial<Type, degree> const& cpy){
                for (size_t i{}; i < coeffs.size(); ++i){
                    coeffs[i] = static_cast<valuetype>(cpy[i]);
                }
            }

            //copy assignment 
            template<typename Type>
            Polynomial& operator=(Polynomial<Type, degree> const& cpy){
                for (size_t i{}; i < coeffs.size(); ++i){
                    coeffs[i] = static_cast<valuetype>(cpy[i]);
                }
                return *this;
            }

            valuetype operator()(valuetype const& a){
                valuetype sum{};
                for (size_t i{}; i < coeffs.size(); ++i){
                    sum += coeffs[i] * power(a,i);
                }
                return sum;
            }
            valueref operator[](size_t idx){
                return coeffs[idx];
            }
            valuetype operator[](size_t const& idx) const{
                return coeffs[idx];
            }
            
        private:
            std::array<valuetype, degree + 1> coeffs;
    };
// declare class template Polynomial
    template <typename T, int degree1, int degree2>
    Polynomial<T, degree1 + degree2> operator*(Polynomial<T, degree1> const& a, Polynomial<T, degree2> const& b){
        Polynomial<T, degree1 + degree2> out{};
        // std::cerr << degree1 << "<deg1 deg2> " << degree2 << std::endl;
        // for (int i {}; i < degree1 + degree2; ++i){
        //     std::cerr << out[i];
        // }
        // std::cerr << std::endl;
        for (int i{}; i < degree1 + 1; ++i){
            for(int j{}; j < degree2 + 1; ++j){
                out[i+j] += a[i] * b[j];
            }
        }
        // for (int i {}; i < degree1 + degree2; ++i){
        //     std::cerr << out[i];
        // }
        return out;
    }
}

#include "polynomial.tpp"

#endif
