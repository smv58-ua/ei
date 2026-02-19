#include <iostream>
#include "tokenizador.h"

using namespace std;

int main()
{
    Tokenizador tokenizador(" ", false, true);

    tokenizador.Tokenizar("input.txt", "output.txt");
}