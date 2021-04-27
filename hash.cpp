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
    result = streamRead(&fin);
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
    fout << _data;
    fout.close();

    system(command.c_str());

    std::ifstream fin(fileOut);
    result = streamRead(&fin);
    fin.close();

    return result;
}
std::string hash::rsaSign(const std::string &_privateKey, const std::string &_data)
{
    std::ofstream fout(fileIn + "1");
    fout << _privateKey;
    fout.close();

    fout.open(fileIn);
    fout << _data;
    fout.close();

    std::string result;
    const std::string command = 
    (std::string)"openssl dgst -sha1 -sign "
    + CURR_PATH 
    + fileIn + "1"
    + (std::string)" -out "
    + CURR_PATH
    + fileOut
    + (std::string)" "
    + fileIn;

    system(command.c_str());

    std::ifstream fin(fileOut);
    result = streamRead(&fin);
    fin.close();

    return result;
}
bool hash::rsaVerify(const std::string &_publicKey, const std::string &_sign, std::string &_data)
{
    std::ofstream fout(fileIn + "1");
    fout << _publicKey;
    fout.close();

    fout.open(fileIn + "2");
    fout << _sign;
    fout.close();

    fout.open(fileIn);
    fout << _data;
    fout.close();

    std::string result;
    const std::string command = 
    (std::string)"openssl dgst -sha1 -verify "
    + CURR_PATH 
    + fileIn + "1"
    + (std::string)" -signature "
    + CURR_PATH
    + fileIn + "2"
    + (std::string)" "
    + fileIn
    + " > "
    + fileOut;

    system(command.c_str());

    std::ifstream fin(fileOut);
    fin >> result >> result;
    fin.close();

    if(result == "OK") return true;
    return false;
}
std::string streamRead(std::istream* strIn)
{
    std::string str = "";
    char buff;

    while(!strIn->eof())
    {
        // takes next symb
        buff = strIn->get();
        
        // adding new sumb
        if(buff != EOF) str += buff;
    }
    return str;
}