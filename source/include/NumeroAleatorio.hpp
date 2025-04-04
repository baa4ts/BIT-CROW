#ifndef NUMEROALEATORIO_HPP
#define NUMEROALEATORIO_HPP

#include <random>

class NumeroAleatorio
{
public:
    static int Generar(int Inicial, int Final)
    {
        static std::random_device dispositivo;
        static std::mt19937 generador(dispositivo());
        static std::uniform_int_distribution<int> distribucion(Inicial, Final);
        return distribucion(generador);
    }
};

#endif
