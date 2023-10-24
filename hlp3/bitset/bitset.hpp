/**
 * @file bitset.hpp
 * @author tan cheng hian (t.chenghian@digipen.edu)
 * @brief assignment 2
 * @version 0.1
 * @date 2023-09-12
 * 
 * @copyright Copyright (c) 2023
 * 
 */
namespace HLP3{
    /**
     * @brief Construct a new bitset<bitsize>::bitset object
     * 
     * @tparam bitsize 
     */
    template <size_t bitsize>
    bitset<bitsize>::bitset() : bits{new char[(bitsize/8) + 1]{}}{}

    /**
     * @brief Destroy the bitset<bitsize>::bitset object
     * 
     * @tparam bitsize 
     */
    template <size_t bitsize>
    bitset<bitsize>::~bitset(){
        delete [] bits;
    }

    /**
     * @brief set bit
     * 
     * @tparam bitsize 
     * @param pos 
     * @param value 
     */
    template <size_t bitsize>
    void bitset<bitsize>::set(size_t pos, bool value){ 
        //std::cerr << pos << std::endl;
        if (pos >= bitsize) throw std::out_of_range{"outofrange"};
        if (value) bits[pos/8] |= (1 << (pos % 8) );
        else bits[pos/8] &= ~(1 << (pos % 8) ); 
    }

    /**
     * @brief reset bit
     * 
     * @tparam bitsize 
     * @param pos 
     */
    template <size_t bitsize>
    void bitset<bitsize>::reset(size_t pos){ bits[pos/8] &= 0 << (pos%8); }

    /**
     * @brief flip bit
     * 
     * @tparam bitsize 
     * @param pos 
     */
    template <size_t bitsize>
    void bitset<bitsize>::flip(size_t pos){ bits[pos/8] ^= 1 << (pos%8);}

    /**
     * @brief test bit
     * 
     * @tparam bitsize 
     * @param pos 
     * @return true 
     * @return false 
     */
    template <size_t bitsize>
    bool bitset<bitsize>::test(size_t pos) const{ 
        if (pos >= bitsize) throw std::out_of_range{"outofrange"};
        return bits[pos/8] & (1 << (pos%8)); 
    }

    /**
     * @brief count bits
     * 
     * @tparam bitsize 
     * @return size_t 
     */
    template <size_t bitsize>
    size_t bitset<bitsize>::count() const{
        size_t count{};
        for (size_t i{}; i < bitsize; ++i) (test(i)) ? ++count : count+=0;
        return count; 
    }

    /**
     * @brief max bits
     * 
     * @tparam bitsize 
     * @return size_t 
     */
    template <size_t bitsize>
    size_t bitset<bitsize>::size() const{return bitsize;}

    /**
     * @brief subscript operator
     * 
     * @tparam bitsize 
     * @param pos 
     * @return true 
     * @return false 
     */
    template <size_t bitsize>
    bool bitset<bitsize>::operator[](size_t pos) const{ return test(pos);}

    /**
     * @brief to string
     * 
     * @tparam bitsize 
     * @param f 
     * @param t 
     * @return std::string 
     */
    template <size_t bitsize>
    std::string bitset<bitsize>::to_string(char f, char t){
        std::string out{};
        for (int i{static_cast<int>(bitsize) - 1}; i >= 0; --i) (test(i)) ? out+=t : out+=f;
        return out; 
    }
}
