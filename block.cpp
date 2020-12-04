#include "block.h"

block::block()
{
    m_data = new std::string();
}
block::block(block &newBlock) : block()
{
    m_prevHash = newBlock.prevHash();
    m_index = newBlock.index();
    m_description = newBlock.description();
    *m_data = newBlock.data();
}   
block::block(const std::string &newPrevHash, const size_t newIndex, const std::string &newDescription, const std::string &newData) : 
    block()
{
    m_prevHash = newPrevHash;
    m_index = newIndex; // genesis block
    m_description = newDescription;
    *m_data = newData;
}
block::block(const std::string &strBlock) :
    block()
{
    this->fromString(strBlock);
}
block::~block()
{
    delete m_data;
}

/* getters */

size_t block::index() const
{
    return m_index;
}
std::string block::prevHash() const
{
    return m_prevHash;
}
std::string block::description() const
{
    return m_description;
}
std::string block::data() const
{
    return *m_data;
}
const std::string* block::pdata() const
{
    return m_data;
}
/* setters */

void block::setPrevHash(const std::string &newPrevHash)
{
    m_prevHash = newPrevHash;
}
void block::setIndex(const size_t &newIndex)
{
    m_index = newIndex;
}
void block::setDescription(const std::string &newDescription)
{
    m_description = newDescription;
}
void block::setData(const std::string &newData)
{
    *(m_data) = newData;
}

/* work with strings */

std::string block::toString() const
{
    //   prevHash  //
    //    index    //
    // description //
    //    data     //
    std::stringstream ss;                 
    ss << m_prevHash << "\n"
    << std::to_string(m_index) << "\n"
    << m_description << "\n"
    << *m_data;
    return ss.str();
}
void block::fromString(const std::string &strBlock)
{
    //   prevHash  //
    //    index    //
    // description //
    //    data     //
    std::stringstream ss; 
    std::string buff;
    char b;
    ss << strBlock;
    m_data->clear();

    ss >> m_prevHash;
    ss >> m_index;
    ss >> m_description;
    while(!ss.eof())
    {
        ss >> buff;
        *m_data += buff;
        b = ss.peek();
        if(ss.peek() != -1) *m_data += b;
    }
}