#include "tokenizador.h"

Tokenizador::Tokenizador(const string& delimitadoresPalabra, const bool& kcasosEspeciales, const bool& minuscSinAcentos)
{

    for (char character : delimitadoresPalabra) {

        this->delimiters.insert(character);    
    }
    
    this->casosEspeciales = casosEspeciales;
    this->pasarAminuscSinAcentos = minuscSinAcentos;
}

Tokenizador::Tokenizador(const Tokenizador& tokenizador)
{
}