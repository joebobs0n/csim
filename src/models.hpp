#pragma once
#ifndef _MODELS_HPP_
#define _MODELS_HPP_

#define VAL const static float

namespace Model {
    inline namespace Planar {
        //! data from : https://rincon-mora.gatech.edu/classes/ece3040/handouts/spice3.pdf
        VAL L =       180e-9f;  // [m]      channel length
        VAL LD =      0.0;      // [m]      gate-source overlap
        VAL LEFF =    L-2*LD;   // [m]      effective channel length due to tcn overlap
        VAL KP =      50e-6f;   // [A/V^2]  transconductance parameter -> also called gm
        VAL VTO =     1.0;      // [V]      threshold voltag
        VAL LAMBDA =  0.1/L;    // [V^-1]   channel length moduliation
        VAL GAMMA =   0.6;      // [V^1/2]  backgate effect
        VAL PHI =     0.8;      // [V]      bulk potential
        VAL TOX =     15e-9f;   // [m]      gate oxide thickness
        VAL CGDO =    5e-10f;   // [F/m]    gate-drain overlap cap
        VAL CGSO =    5e-10f;   // [F/m]    gate-source overlap cap
        VAL CJ =      1e-4f;    // [F/m^2]  zero-bias planar substrate depletion cap
        VAL CJSW =    5e-10f;   // [F/m]    zero-bias sidewall substrate depletion cap
        VAL PB =      0.95;     // [V]      Substrate junction potential -> also called phi_B
        VAL MJ =      0.5;      // [-]      planar substrate junction grading coeff
        VAL MJSW =    0.33;     // [-]      sidewall substrate junction grading coeff
    }

    namespace Fin {}
    namespace GAA {}

    namespace BJT {}
}

#endif
