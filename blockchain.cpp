#include "blockchain.h"


blockchain::blockchain(const std::string& _blockchainPath) : m_blockchainPath(_blockchainPath), m_lastIndex(0)
{
    // мб бч уже есть
    m_lastIndex = this->isAlreadyExist();

    // мб тут проверка на то, есть ли что-то у других пользователей
    // ..
}
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
    block new_block{};

    // создать блок, когда в нем нет ген блока? Вернем пустой какой-то блок
    if(m_lastIndex == INDEX_NO_GEN_BLOCK) return block();
    
    // блоки здоровые пипец, так что работаем с их хэшами
    std::string hashed_data = hash::sha1(_data);

    // из этого создадим bind_hash (связующий хэш палучаеца)
    std::string hashed_last_block = hash::sha1(this->getBlock(m_lastIndex).toString());

    // взяли кароче ласт блок, присоеденили к нему _data от текущего блока, получили новый хэшик
    std::string bind_hash = hash::sha1(hashed_last_block + hashed_data);

    // создали сигну(подпись кароче) - она уже в base64, так что проблем с записью в файл не будет
    std::string sign = hash::rsaSign(_privateKey, hashed_data);

    new_block.setPublicKey(_publicKey);
    new_block.setIndex(m_lastIndex + 1);
    new_block.setBindHash(bind_hash);
    new_block.setSign(sign);
    new_block.setData(_data);

    return new_block;
}
int blockchain::addBlock(const block &_new_block)
{
    if(_new_block.index() != m_lastIndex + 1 || m_lastIndex == INDEX_NO_GEN_BLOCK)      return -3;
    if(!isSignValid(_new_block))                                                        return -2;
    if(!isBindHashValid(_new_block))                                                    return -1;

    // проверки прошли, пишем в файл
    m_lastIndex++;
    this->writeBlock(&_new_block);
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

    block prev_block(this->getBlock(_block.index() - 1));

    // по факту просто делаем тоже самое, что и делали бы, если создавали новый блок на основе предыдущего
    std::string hashed_data = hash::sha1(_block.data());
    std::string hashed_last_block = hash::sha1(prev_block.toString());
    std::string bind_hash = hash::sha1(hashed_last_block + hashed_data);

    // если все совпало - кайфуем
    if(_block.bindHash() == bind_hash) return true;
    return false;
}
size_t blockchain::isBindHashValidAll() const
{
    // проверяем все блоки начиная с 0
    if(m_lastIndex == INDEX_NO_GEN_BLOCK) return 0;
    for(size_t i = 0; i < m_lastIndex; i++) if(!this->isBindHashValid(i)) return i;
    return 0;
}
bool blockchain::isSignValid(size_t _index) const
{
    return this->isSignValid(this->getBlock(_index));
}
bool blockchain::isSignValid(const block &_block) const
{
    // взяли сигну, взяли дату, взяли ключик публичный и проверили, как оно там
    std::string sign = _block.sign();
    std::string hashed_data = hash::sha1(_block.data());

    return hash::rsaVerify(_block.publicKey(), sign, hashed_data);
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
    // если ген блок уже есть то выходим
    if(m_lastIndex != INDEX_NO_GEN_BLOCK) return;

    block new_block;

    // ген блок все дела
    m_lastIndex = 0;

    // все через хэши, а то обычная дата мб большой
    std::string hashedData = hash::sha1(_genData);

    // создали сигну(подпись кароче) - она в base64 сразу ес че
    std::string sign = hash::rsaSign(_genPrivateKey, hashedData);

    new_block.setPublicKey(_genPublicKey);
    new_block.setIndex(m_lastIndex);
    new_block.setBindHash(_genBindHash);
    new_block.setSign(sign);
    new_block.setData(_genData);

    // записали в файлик сразу
    this->writeBlock(&new_block);
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
    if(i == 0) return INDEX_NO_GEN_BLOCK;
    return i - 1;
}

