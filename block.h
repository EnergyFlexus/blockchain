#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <ctime>
#include <sstream>
#include <fstream>

#include "hash.h"

class block
{
    public:

    block(const std::string &description, const std::string &data); // genesis block
    block(const block *prevBlock, const std::string &description, const std::string &data);
    explicit block(const std::string &strBlock); // get block from string
    ~block();

    /* getters */
    size_t index() const;
    std::string prevHash() const;
    std::string description() const;
    std::string data() const;

    /* setters */
    void setDescription(const std::string &description);
    void setData(const std::string &data);

    /* work with strings */
    std::string toString() const;

    /* work with files */
    void writeFile(const std::string &blockPath) const;

    private:
    const block *m_prevBlock;
    std::string m_prevHash;
    size_t m_index;
    std::string m_description;
    std::string *m_data;
};

#endif // BLOCK_H