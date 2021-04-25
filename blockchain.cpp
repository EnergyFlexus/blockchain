#include "blockchain.h"


blockchain::blockchain(const std::string &_blockchainPath, const std::string &_description, const std::string &_data) : 
    m_blockchainPath(_blockchainPath)
{
    // мб бч уже есть, надо почекать че там, ток проверку на валидность не делаю, тут уж сами решайте когда и где ее делать
    m_lastIndex = this->isAlreadyExist();

    // ну если блоков нет то делаем генезисный и кайфуем
    if(!m_lastIndex) this->addGenBlock(_description, _data);
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
    size_t i = m_lastIndex;
    for(; i > m_lastIndex - _count && i != 0; i--) 
        remove(std::string(m_blockchainPath + std::to_string(i) + EXTENSION).c_str());
    m_lastIndex = i;
}
size_t blockchain::isValid()
{
    // с нуля пересчитываем хэши и сравниваем, где хэш не тот то и возвращаем, если все ок то 0
    for(size_t i = 1; i <= m_lastIndex; i++) if(hash::sha1(getFile(i - 1)) != getHash(i)) return i;
    return 0;
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
size_t blockchain::isAlreadyExist()
{
    size_t i = 0;
    struct stat b;
    // вот этот вот stat возвращает -1 если файла нет, но ему еще надо заполнить struct stat, так что ток так
    for(;;i++) if(stat(std::string(m_blockchainPath + std::to_string(i) + EXTENSION).c_str(), &b) == -1) break;
    return i - 1;
}
std::string blockchain::getHash(size_t _index)
{
    std::string filename = m_blockchainPath + std::to_string(_index) + EXTENSION;
    std::string result;
    std::ifstream fin(filename);
        
    if(!fin.is_open())
    {
        std::cerr << "Err. Is path to blockchain is valid?";
        exit(1);
    }

    fin >> result;
    return result;
}
std::string blockchain::getFile(size_t _index)
{
    std::string filename = m_blockchainPath + std::to_string(_index) + EXTENSION;
    std::string result;
    std::ifstream fin(filename);

    if(!fin.is_open())
    {
        std::cerr << "Err. Is path to blockchain is valid?";
        exit(1);
    }

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
