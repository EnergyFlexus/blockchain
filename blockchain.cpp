#include "blockchain.h"
#include <iostream>

blockchain::blockchain(const std::string &_blockchainPath, const std::string &_description, const std::string &_data) : 
    m_blockchainPath(_blockchainPath)
{
    m_lastIndex = 0;
    this->addGenBlock(_description, _data);
}
blockchain::~blockchain(){}
std::string blockchain::blockchainPath() const
{
    return m_blockchainPath;
}
size_t blockchain::lastIndex() const
{
    return m_lastIndex;
}
void blockchain::addBlock(const std::string &_description, const std::string &_data)
{
    block *newBlock = new block();
    std::string prevBlockStr = this->getFile(m_lastIndex);

    newBlock->setPrevHash(hash::sha1(prevBlockStr));
    newBlock->setIndex(m_lastIndex + 1);
    newBlock->setDescription(_description);
    newBlock->setData(_data);

    writeBlock(newBlock);
    m_lastIndex++;
    delete newBlock;
}
void blockchain::deleteBlocks(const size_t _count)
{
    for(; m_lastIndex >= _count; m_lastIndex--) 
        remove(std::string(m_blockchainPath + std::to_string(m_lastIndex) + EXTENSION).c_str());
}
void blockchain::addGenBlock(const std::string &_description, const std::string &_data)
{
    block *new_block = new block();

    new_block->setPrevHash("");
    new_block->setIndex(0); // genesis block ofc
    new_block->setDescription(_description);
    new_block->setData(_data);

    writeBlock(new_block);
    delete new_block;
}
void blockchain::writeBlock(const block *_block)
{
    std::string filename = m_blockchainPath + std::to_string(_block->index()) + EXTENSION;
    std::ofstream fout(filename);
    fout << _block->toString();
    fout.close();
}
std::string blockchain::getHash(size_t _index)
{
    std::string filename = m_blockchainPath + std::to_string(_index) + EXTENSION;
    std::string result;
    std::ifstream fin(filename);
    fin >> result;
    return result;
}
std::string blockchain::getFile(size_t _index)
{
    std::string filename = m_blockchainPath + std::to_string(_index) + EXTENSION;
    std::string result;
    std::ifstream fin(filename);
    std::string buff;
    char b;
    while(!fin.eof())
    {
        fin >> buff;
        result += buff;
        b = fin.peek();
        if(fin.peek() != -1) result += b;
    }
    return result;
}
