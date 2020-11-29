#include "blockchain.h"
#include <iostream>

blockchain::blockchain(const std::string &newBlockchainPath, const std::string &newDescription, const std::string &newData) : 
    m_blockchainPath(newBlockchainPath)
{
    m_lastIndex = 0;
    this->addGenBlock(newDescription, newData);
}
blockchain::~blockchain(){}
void blockchain::addBlock(const std::string &newDescription, const std::string &newData)
{
    block *newBlock = new block();
    std::string prevBlockStr = this->getFile(m_lastIndex);

    newBlock->setPrevHash(hash::sha1(prevBlockStr));
    newBlock->setIndex(m_lastIndex + 1);
    newBlock->setDescription(newDescription);
    newBlock->setData(newData);

    writeBlock(newBlock);
    m_lastIndex++;
    delete newBlock;
}
void blockchain::addGenBlock(const std::string &newDescription, const std::string &newData)
{
    block *newBlock = new block();

    newBlock->setPrevHash("");
    newBlock->setIndex(m_lastIndex); // m_lastIndex = 0
    newBlock->setDescription(newDescription);
    newBlock->setData(newData);

    writeBlock(newBlock);
    delete newBlock;
}
void blockchain::writeBlock(const block *blockToWrite)
{
    std::string filename = m_blockchainPath + std::to_string(blockToWrite->index()) + ".txt";
    std::ofstream fout(filename);
    fout << blockToWrite->toString();
    fout.close();
}
std::string blockchain::getHash(size_t index)
{
    std::string filename = m_blockchainPath + std::to_string(index) + ".txt";
    std::string result;
    std::ifstream fin(filename);
    fin >> result;
    return result;
}
std::string blockchain::getFile(size_t index)
{
    std::string filename = m_blockchainPath + std::to_string(index) + ".txt";
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
