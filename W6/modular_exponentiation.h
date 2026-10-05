//
// Created by Yahya Muayyiduddin on 13/04/26.
//

#include <bitset>

#ifndef W6_MODULAR_EXPONENTIATION_H
#define W6_MODULAR_EXPONENTIATION_H

#endif //W6_MODULAR_EXPONENTIATION_H


namespace week6 {

    /**
     * a ^c1p + c2*v = a^c1*p * a^c2*v = x^p^c1 * x^v^c2
     *
     *
     * property: x*y % z = (x % z * y % z) % z
     *
     * @param a
     * @param b
     * @param mod
     * @return
     */
    int repeated_squaring(int a,int b,  int mod){

        std::bitset<64> input(a);
    }



}