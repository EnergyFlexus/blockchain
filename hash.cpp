#include "hash.h"

// т.к я рот ебал подключать библы то делаю все ультра на костылях через cmd

const std::string fileIn = (std::string)TEMP_PATH + "tempIn";
const std::string fileOut = (std::string)TEMP_PATH + "tempOut";

std::string hash::sha1(const std::string &_data)
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
    fout << _data;
    fout.close();

    system(command.c_str());

    std::ifstream fin(fileOut);
    fin >> result >> result;
    fin.close();

    return result;
}
std::string hash::base64Encode(const std::string &_data)
{
    std::string result;
    const std::string command = 
    (std::string)"openssl base64 -in "
    + CURR_PATH 
    + fileIn
    + (std::string)" -out "
    + CURR_PATH
    + fileOut;

    std::ofstream fout(fileIn);
    fout << _data;
    fout.close();

    system(command.c_str());

    std::ifstream fin(fileOut);
    fin >> result;
    fin.close();

    return result;
}
std::string hash::base64Decode(const std::string &_data)
{
    std::string result;
    const std::string command = 
    (std::string)"openssl base64 -d -in "
    + CURR_PATH 
    + fileIn
    + (std::string)" -out "
    + CURR_PATH
    + fileOut;

    std::ofstream fout(fileIn);
    fout << _data << '\n';
    fout.close();

    system(command.c_str());

    std::ifstream fin(fileOut);
    fin >> result;
    fin.close();

    return result;
}
std::string hash::rsaSign(const std::string &_privateKey, const std::string &_data)
{
    return "1";
}