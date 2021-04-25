#include "block.h"

//   prevHash  //
//    index    //
// description //
//    data     //

block::block()
{
}
block::block(const block &_block)
{
    m_prevHash = _block.prevHash();
    m_index = _block.index();
    m_description = _block.description();
    m_data = _block.data();
}   
block::block(const std::string &_prevHash, const size_t _index, const std::string &_description, const std::string &_data)
{
    m_prevHash = _prevHash;
    m_index = _index;
    m_description = _description;
    m_data = _data;
}
block::block(const std::string &strBlock)
{
    this->fromString(strBlock);
}
block::~block()
{
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
    return m_data;
}

/* setters */

void block::setPrevHash(const std::string &_prevHash)
{
    m_prevHash = _prevHash;
}
void block::setIndex(const size_t _index)
{
    m_index = _index;
}
void block::setDescription(const std::string &_description)
{
    m_description = _description;
}
void block::setData(const std::string &_data)
{
    m_data = _data;
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
    << m_data;
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
    m_data.clear();

    ss >> m_prevHash;
    ss >> m_index;
    ss >> m_description;
    while(!ss.eof())
    {
        ss >> buff;
        m_data += buff;
        b = ss.peek();
        if(ss.peek() != -1) m_data += b;
    }
}