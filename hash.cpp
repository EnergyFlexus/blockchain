#include "hash.h"

// т.к я рот ебал подключать библы то делаю все ультра на костылях через cmd

const std::string fileIn = (std::string)TEMP_PATH + "tempIn";
const std::string fileOut = (std::string)TEMP_PATH + "tempOut";

std::string hash::sha1(const std::string &data)
{
    std::string result;
    const std::string command = 
    (std::string)"openssl sha1 "
    + CURR_PATH 
    + fileIn
    + (std::string)" > "
    + CURR_PATH
    + fileOut;

    std::ofstream fout(fileIn);
    fout << data;
    fout.close();

    system(command.c_str());

    std::ifstream fin(fileOut);
    fin >> result >> result;
    fin.close();

    return result;
}