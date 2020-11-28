#include "block.h"

block::block(const std::string &description, const std::string &data)
{
    m_prevBlock = nullptr; 
    m_prevHash = "";
    m_index = 0; // genesis block
    m_description = description;
    m_data = new std::string(); *m_data = data;
}
block::block(const block *prevBlock, const std::string &description, const std::string &data) : block(description, data)
{
    m_prevBlock = prevBlock;
    m_prevHash = hash::sha1(m_prevBlock->toString());
    m_index = m_prevBlock->index() + 1;
}
block::block(const std::string &strBlock)
{
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
    ss >> *m_data;
}
block::~block()
{
    delete m_data;
}
size_t block::index() const
{
    return this->m_index;
}
std::string block::prevHash() const
{
    return this->m_prevHash;
}
std::string block::description() const
{
    return this->m_description;
}
std::string block::data() const
{
    return *this->m_data;
}
void block::setDescription(const std::string &description)
{
    m_description = description;
}
void block::setData(const std::string &data)
{
    *(m_data) = data;
}
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
void block::writeFile(const std::string &blockPath) const
{
    const std::string filename = blockPath + std::to_string(m_index) + ".txt";
    std::ofstream fout(filename);
    fout << this->toString();
    fout.close();
}