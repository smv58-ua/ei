#include "tokenizador.h"
#include <fstream>

// CONSTRUCTORES

Tokenizador::Tokenizador() 
    : Tokenizador(",;:.-/+*\\ '\"{}[]()<>?!??&#=\t@", true, false)
{}

Tokenizador::Tokenizador(const string& delimitadoresPalabra, const bool& kcasosEspeciales, const bool& minuscSinAcentos)
    : delimiters(delimitadoresPalabra), delimitersSet(delimitadoresPalabra.begin(), delimitadoresPalabra.end()), casosEspeciales(kcasosEspeciales), pasarAminuscSinAcentos(minuscSinAcentos)
{}

Tokenizador::Tokenizador(const Tokenizador& tokenizador)
    : delimiters(tokenizador.delimiters), delimitersSet(tokenizador.delimitersSet), casosEspeciales(tokenizador.casosEspeciales), pasarAminuscSinAcentos(tokenizador.pasarAminuscSinAcentos)
{}

// DESTRUCTORES

//  Destruye en objeto Tokenizador, limpiando el contenido de "delimiters".
Tokenizador::~Tokenizador()
{
    this->delimitersSet.clear();
}

// FUNCIONES DE TOKENIZAR

void Tokenizador::Tokenizar(const string& str, list<string>& tokens) const
{
    tokens.clear();

    string cadena;
    if (this->pasarAminuscSinAcentos)
        cadena = PasarAMinuscSinAcentos(str);
    else
        cadena = str;
    
    size_t substring_start = 0;

    for (size_t iterator = 0; iterator < cadena.size(); ++iterator) {

        if (this->delimitersSet.find(cadena[iterator]) != this->delimitersSet.end()) {
            
            if (iterator > substring_start)
                tokens.push_back(cadena.substr(substring_start, iterator - substring_start));
            
            substring_start = iterator + 1;
        }
    }
    
    if (substring_start < cadena.size())
        tokens.push_back(cadena.substr(substring_start));

}

bool Tokenizador::Tokenizar(const string& i, const string& f) const
{

    const size_t BUFFER_SIZE = 2;

    try {

        ifstream inputFile(i.c_str());

        if (!inputFile.is_open()) {
            cerr << "Error: No se pudo abrir el archivo de entrada '" << i << "'." << endl;
            return false;
        }

        ofstream outputFile(f.c_str());

        if (!outputFile.is_open()) {
            cerr << "Error: No se pudo abrir el archivo de salida '" << f << "'." << endl;
            return false;
        }

        string line;
        line.resize(BUFFER_SIZE);
        bool eof = !inputFile.read(line.data(), BUFFER_SIZE);

        while(!eof) {
            
            list<string> tokens;
            TokenizarFichero(line.substr(0, inputFile.gcount()), tokens, eof);

            for (const string& token : tokens) {
                outputFile << token << endl;
            }

            eof = !inputFile.read(line.data(), BUFFER_SIZE);
        }

        list<string> tokens;
        TokenizarFichero(line.substr(0, inputFile.gcount()), tokens, true);
        
        for (const string& token : tokens) {
            outputFile << token << endl;
        }

        inputFile.close();
        outputFile.close();

        return true;

    } catch (const exception& e) {
        cerr << "Error: Ocurrió una excepción al procesar los archivos. Detalles: " << e.what() << endl;
    }

    return false;
}

// GETTERS

//  Devuelve "delimiters"
string Tokenizador::DelimitadoresPalabra() const
{
    return DelimitesToString();
}

//  Devuelve el contenido de la variable privada "casosEspeciales"
bool Tokenizador::CasosEspeciales() const
{
    return this->casosEspeciales;
}

//  Devuelve el contenido de la variable privada "pasarAminuscSinAcentos"
bool Tokenizador::PasarAminuscSinAcentos() const
{
    return this->pasarAminuscSinAcentos;
}

// SETTERS

//  Inicializa delimiters a nuevoDelimiters, filtrando que no se introduzcan delimitadores repetidos
//  (de izquierda a derecha, en cuyo caso se eliminar?n los que hayan sido repetidos por la derecha).
//
//  Parametros:
//      nuevoDelimiters: String con los nuevos delimitadores.
void Tokenizador::DelimitadoresPalabra(const string& nuevoDelimiters)
{
    this->delimitersSet.clear();
    this->delimiters.clear();
    this->AnyadirDelimitadoresPalabra(nuevoDelimiters);
}

//  A?ade al final de "delimiters" los nuevos delimitadores que aparezcan en "nuevoDelimiters" (no se 
//  almacenar?n caracteres repetidos).
//
//  Parametros:
//      nuevoDelimiters: String con los nuevos delimitadores a a?adir.
void Tokenizador::AnyadirDelimitadoresPalabra(const string& nuevoDelimiters)
{
    delimiters += nuevoDelimiters;

    for (char character : nuevoDelimiters) {
        this->delimitersSet.insert(character);
    }
}

//  Cambia la variable privada "casosEspeciales".
//
//  Parametros:
//      nuevoCasosEspeciales: Nuevo valor para la variable privada "casosEspeciales".
void Tokenizador::CasosEspeciales(const bool& nuevoCasosEspeciales)
{
    this->casosEspeciales = nuevoCasosEspeciales;
}

//  Cambia la variable privada "pasarAminuscSinAcentos". Atenci?n al formato de codificaci?n del corpus
//  (comando "file" de Linux). Para la correcci?n de la pr?ctica se utilizar? el formato actual (ISO-8859).
//
//  Parametros:
//      nuevoPasarAminuscSinAcentos: Nuevo valor para la variable privada "pasarAminuscSinAcentos".
void Tokenizador::PasarAminuscSinAcentos(const bool& nuevoPasarAminuscSinAcentos)
{
    this->pasarAminuscSinAcentos = nuevoPasarAminuscSinAcentos;
}

// FUNCIONES AUXILIARES

//  Devuelve un string con los caracteres de delimiters en el orden en que se introdujeron (de izquierda
//  a derecha, eliminando los caracteres repetidos por la derecha).
string Tokenizador::DelimitesToString() const
{
    return delimiters;
}

string Tokenizador::PasarAMinuscSinAcentos(const string& str) const
{

    static const locale loc("es_ES.ISO-8859-1");

    static const char ISO_8859_1_ACENTOS[32] = {
        'a', 'a', 'a', 'a', 'a', 'a',       // 0xE0 - 0xE5 (à, á, â, ã, ä, å)
        'æ', 'c',                           // 0xE6 - 0xE7 (æ, ç) -> Se omiten
        'e', 'e', 'e', 'e',                 // 0xE8 - 0xEB (è, é, ê, ë)
        'i', 'i', 'i', 'i',                 // 0xEC - 0xEF (ì, í, î, ï)
        'ð', 'ñ',                           // 0xF0 - 0xF1 (ð, ñ) -> Se omiten
        'o', 'o', 'o', 'o', 'o',            // 0xF2 - 0xF6 (ò, ó, ô, õ, ö)
        '÷', 'ø',                           // 0xF7 - 0xF8 (÷, ø) -> Se omiten
        'u', 'u', 'u', 'u',                 // 0xF9 - 0xFC (ù, ú, û, ü)
        'y', 'þ', 'y'                       // 0xFD - 0xFF (ý, þ, ÿ) -> Se omiten
    };

    string resultado = str;

    for (char& character : resultado) {
        
        character = tolower(character, loc);

        unsigned char c = static_cast<unsigned char>(character);
        
        if (c >= 0xE0 && c <= 0xFF)
            character = ISO_8859_1_ACENTOS[c - 0xE0];
    }

    return resultado;
}

void Tokenizador::TokenizarFichero(const string& str, list<string>& tokens, const bool eof) const
{
    static string tokenAnterior = "";  // Variable est?tica para almacenar el token que pueda haberse quedado sin completar en la llamada anterior
    tokens.clear();

    string cadena;
    if (this->pasarAminuscSinAcentos)
        cadena = PasarAMinuscSinAcentos(str);
    else
        cadena = str;
    
    cadena = tokenAnterior + cadena;         // Concatenar el token anterior con la nueva cadena a tokenizar
    size_t iterator = tokenAnterior.size();  // Empezar a iterar desde el punto donde se quedó el token anterior
    tokenAnterior.clear();                    // Limpiar el token anterior para la próxima llamada
    size_t substring_start = 0;

    for (; iterator < cadena.size(); ++iterator) {

        if (this->delimitersSet.find(cadena[iterator]) != this->delimitersSet.end()) {
            
            if (iterator > substring_start)
                tokens.push_back(cadena.substr(substring_start, iterator - substring_start));
            
            substring_start = iterator + 1;
        }
    }
    
    if (substring_start < cadena.size())
        if (eof) {
            tokens.push_back(cadena.substr(substring_start));  // Si es el final del archivo, agregar el token restante
            tokenAnterior.clear();
        } else
            tokenAnterior = cadena.substr(substring_start);
}