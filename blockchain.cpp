#include "blockchain.h"


blockchain::blockchain(const std::string &_blockchainPath,
    const std::string &_genPublicKey,
    const std::string &_genPrivateKey,
    const std::string &_genBindHash,
    const std::string &_genData) : m_blockchainPath(_blockchainPath)
{
    // мб бч уже есть, надо почекать че там + проверяем хэши, вдруг напакостили
    m_lastIndex = this->isAlreadyExist();
    if(m_lastIndex) return;

    // ну если блоков нет то делаем генезисный и кайфуем
    this->createGenBlock(_genPublicKey, _genPrivateKey, _genBindHash, _genData);
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
block blockchain::createBlock(const std::string &_publicKey,
    const std::string &_privateKey,
    const std::string &_data)
{
    block new_block;
    
    // блоки здоровые пипец, так что работаем с их хэшами
    std::string hashedData = hash::sha1(_data);
    std::string hashedLastBlock = hash::sha1(this->getBlock(m_lastIndex).toString());

    // взяли кароче ласт блок, присоеденили к нему _data от текущего блока, получили новый хэшик
    std::string bindHash = hash::sha1(hashedLastBlock + hashedData);

    // создали сигну(подпись кароче) и бахнули ее в base64, ибо там без этого будет фулл пиздец, а так хотяб символы какие-то
    // создаем ее на основе хешированных данных, ибо _data большой очень
    std::string sign = hash::rsaSign(_privateKey, hashedData);
    sign = hash::base64Encode(sign);

    new_block.setPublicKey(_publicKey);
    new_block.setIndex(m_lastIndex + 1);
    new_block.setBindHash(bindHash);
    new_block.setSign(sign);
    new_block.setData(_data);

    return new_block;
}
// 0 - все ок; 1 - хэши в говне; 2 - индексы в говне; -1 - эцп в говне;
int blockchain::addBlock(const block &_new_block)
{
    if(_new_block.index() != m_lastIndex + 1)   return 2;
    if(!isBindHashValid(_new_block))            return 1;
    if(!isSignValid(_new_block))                return -1;

    m_lastIndex++;

    writeBlock(&_new_block);
    return 0;
}
void blockchain::deleteBlocks(const size_t _count)
{
    size_t i = m_lastIndex;
    for(; i > m_lastIndex - _count && i != 0; i--) 
        remove(std::string(m_blockchainPath + std::to_string(i) + EXTENSION).c_str());
    m_lastIndex = i;
}
bool blockchain::isBindHashValid(size_t _index) const
{
    return this->isBindHashValid(this->getBlock(_index));
}
bool blockchain::isBindHashValid(const block &_block) const
{
    if(_block.index() == 0) return true; // если блок генезисный, то че там, хэш всегда верный

    block *prev_block = new block(this->getBlock(_block.index() - 1));

    std::string hashedData = hash::sha1(_block.data());
    std::string hashedLastBlock = hash::sha1(prev_block->toString());
    std::string bindHash = hash::sha1(hashedLastBlock + hashedData);

    delete prev_block;

    if(_block.bindHash() == bindHash) return true;
    return false;
}
size_t blockchain::isAllBindHashValid() const
{
    for(size_t i = 0; i < m_lastIndex; i++) if(!this->isBindHashValid(i)) return i;
    return 0;
}
bool blockchain::isSignValid(size_t _index) const
{
    return this->isSignValid(this->getBlock(_index));
}
bool blockchain::isSignValid(const block &_block) const
{
    std::string sign = hash::base64Decode(_block.sign());
    std::string hashedData = hash::sha1(_block.data());

    return hash::rsaVerify(_block.publicKey(), sign, hashedData);
}
size_t blockchain::isAllSignValid() const
{
    for(size_t i = 0; i < m_lastIndex; i++) if(!this->isSignValid(i)) return i;
    return 0;
}
block blockchain::getBlock(size_t _index) const
{
    std::string filename = m_blockchainPath + std::to_string(_index) + EXTENSION;
    std::string buff;
    block result;
    std::ifstream fin(filename);

    buff = streamRead(&fin);
    fin.close();
    result.fromString(buff);

    return result;
}
void blockchain::createGenBlock(const std::string &_genPublicKey,
    const std::string &_genPrivateKey,
    const std::string &_genBindHash,
    const std::string &_genData)
{
    block *new_block = new block();

    // ген блок все дела
    m_lastIndex = 0;

    // все через хэши, а то обычная дата мб большой
    std::string hashedData = hash::sha1(_genData);

    // создали сигну(подпись кароче) и бахнули ее в base64, ибо там без этого будет фулл пиздец, а так хотяб символы какие-то
    std::string sign = hash::rsaSign(_genPrivateKey, hashedData);
    sign = hash::base64Encode(sign);

    new_block->setPublicKey(_genPublicKey);
    new_block->setIndex(m_lastIndex);
    new_block->setBindHash(_genBindHash);
    new_block->setSign(sign);
    new_block->setData(_genData);

    // записали в файлик сразу
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
    if(i == 0) return 0;
    return i - 1;
}

