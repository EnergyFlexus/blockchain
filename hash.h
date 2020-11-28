#ifndef HASH_H
#define HASH_H

#include <cstdlib>
#include <fstream>

#define CURR_PATH "%cd%/"
#define TEMP_PATH "temp/"

namespace hash
{
    std::string sha1(const std::string &data);
}

#endif // HASH_H