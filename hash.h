#ifndef HASH_H
#define HASH_H

#include <cstdlib>
#include <fstream>

// tmp - временные файлы, curr очевидно текущий
#define CURR_PATH "%cd%/" 
#define TEMP_PATH "temp/"

namespace hash
{
    std::string sha1(const std::string &_data);

    std::string base64Encode(const std::string &_data);
    std::string base64Decode(const std::string &_data);

    std::string rsaSign(const std::string &_privateKey, const std::string &_data);
    bool rsaVerify(const std::string &_publicKey, const std::string &_sign, std::string &_data);
}

std::string streamRead(std::istream* strIn);

#endif // HASH_H