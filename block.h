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
    block(const block&);
    block(const std::string&, const size_t , const std::string&, const std::string&);
    explicit block(const std::string&); // get block from string
    ~block();

    /* getters */
    size_t index() const;
    std::string prevHash() const;
    std::string description() const;
    std::string data() const;

    /* setters */
    void setPrevHash(const std::string&);
    void setIndex(const size_t);
    void setDescription(const std::string&);
    void setData(const std::string&);

    /* work with strings */
    std::string toString() const;
    void fromString(const std::string&);

    private:
    std::string m_prevHash;
    size_t m_index;
    std::string m_description;
    std::string m_data;
};

#endif // BLOCK_H