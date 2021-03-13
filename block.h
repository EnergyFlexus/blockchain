#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <ctime>
#include <sstream>
#include <fstream>

class block
{
    public:

    block();
    block(const block &newBlock);
    block(const std::string &newPrevHash, const size_t newIndex, const std::string &newDescription, const std::string &newData);
    explicit block(const std::string &strBlock); // get block from string
    ~block();

    /* getters */
    size_t index() const;
    std::string prevHash() const;
    std::string description() const;
    std::string data() const;

    /* setters */
    void setPrevHash(const std::string &newPrevHash);
    void setIndex(const size_t &newIndex);
    void setDescription(const std::string &newDescription);
    void setData(const std::string &newData);

    /* work with strings */
    std::string toString() const;
    void fromString(const std::string &strBlock);

    private:
    std::string m_prevHash;
    size_t m_index;
    std::string m_description;
    std::string m_data;
};

#endif // BLOCK_H