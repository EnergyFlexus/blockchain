#ifndef HASH_H
#define HASH_H

#include <cstdlib>
#include <fstream>

// tmp - временные файлы, curr очевидно текущий
#define CURR_PATH "%cd%/" 
#define TEMP_PATH "temp/"

namespace hash
{
    // _data шифруем в sha1
    std::string sha1(const std::string &_data);

    // base64, Encode - шифорвка
    std::string base64Encode(const std::string &_data);

    // base64, Decode - расшифровка
    std::string base64Decode(const std::string &_data);

    // создает подпись на основе пр ключа и каких-то данных
    std::string rsaSign(const std::string &_privateKey, const std::string &_data);

    // подтверждает, что подпись верна на основе пб ключа, самой подписи и данных, которые подписывали
    bool rsaVerify(const std::string &_publicKey, const std::string &_sign, const std::string &_data);
}

std::string streamRead(std::istream* strIn);

#endif // HASH_H