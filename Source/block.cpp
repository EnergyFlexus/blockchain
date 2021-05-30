#include "block.h"

//  publicKey   //
//    index     //
//   bindHash   //
//     sign     //
//     data     //

block::block(const block &_block)
{
    m_publicKey = _block.m_publicKey;
    m_index = _block.m_index;
    m_bindHash = _block.m_bindHash;
    m_sign = _block.m_sign;
    m_data = _block.m_data;
}
block::block(const std::string &_str)
{
    this->fromString(_str);
}

std::string block::publicKey() const
{
    return m_publicKey;
}
size_t block::index() const
{
    return m_index;
}
std::string block::bindHash() const
{
    return m_bindHash;
}
std::string block::sign() const
{
    return m_sign;
}
std::string block::data() const
{
    return m_data;
}

void block::setPublicKey(const std::string &_publicKey)
{
    m_publicKey = _publicKey;
}
void block::setIndex(const size_t _index)
{
    m_index = _index;
}
void block::setBindHash(const std::string &_bindHash)
{
    m_bindHash = _bindHash;
}
void block::setSign(const std::string &_sign)
{
    m_sign = _sign;
}
void block::setData(const std::string &_data)
{
    m_data = _data;
}

std::string block::toString() const
{
    std::string result{};

    result = 
    PUBLIC_KEY  + m_publicKey               + '\n' +
    INDEX       + std::to_string(m_index)   + '\n' +
    BIND_HASH   + m_bindHash                + '\n' +
    SIGN        + m_sign                    + '\n' +
    DATA        + m_data;

    return result;
}
void block::fromString(const std::string &_str)
{
    std::string markers[5]{PUBLIC_KEY, INDEX, BIND_HASH, SIGN, DATA};
    std::string val[5]{};
    size_t pos1 = 0, pos2 = 0;
    for(size_t i = 0; i < 5; i++)
    {
        pos1 = _str.find(markers[i]) + markers[i].size();
        if(i != 4) pos2 = _str.find(markers[i + 1]);
        else pos2 = _str.size() + 1;
        val[i] = _str.substr(pos1, pos2 - pos1 - 1);
    }

    m_publicKey = val[0];
    m_index = atoi(val[1].c_str());
    m_bindHash = val[2];
    m_sign = val[3];
    m_data = val[4];
}