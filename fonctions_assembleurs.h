#ifndef FONCTIONS_ASSEMBLEURS_H
    #define FONCTIONS_ASSEMBLEURS_H

    // execution des instructions arithmétiques et logiques

    void add(short registre[], short decimal[]);

    void sub(short registre[], short decimal[]);

    void mul(short registre[], short decimal[]);

    void DIV(short registre[], short decimal[]);

    void AND(short registre[], short decimal[]);

    void OR(short registre[], short decimal[]);

    void XOR(short registre[], short decimal[]);

    void shl(short registre[], short decimal[]);


    // execution des instructions de transfert


    void ldb(short registre[], char memoire[], short decimal[]);

    void ldw(short registre[], char memoire[], short decimal[]);

    void stb(short registre[], char memoire[], short decimal[]);

    void stw(short registre[], char memoire[], short decimal[]);


    // execution des instructions de sauts

    void jmp(short decimal[]);

    void jzs(short decimal[]);

    void jzc(short decimal[]);

    void jcs(short decimal[]);

    void jcc(short decimal[]);

    void jns(short decimal[]);

    void jnc(short decimal[]);


    // execution entree-sortie

    void in(short registre[], short decimal[]);

    void out(short registre[], short decimal[]);

    // execution d'instructions diverses

    void rnd(short registre[], short decimal[]);

#endif