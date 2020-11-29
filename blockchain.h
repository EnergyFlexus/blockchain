#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "block.h"
#include "hash.h"

#define GENESIS_DESC "genesis block"
#define GENESIS_DATA "Hello, blockchain!"

/* TODO: checking hashes, checking existing chain, ... */

class blockchain
{
    public:

    explicit blockchain(const std::string &newBlockchainPath, 
        const std::string &newDescription = GENESIS_DESC, 
        const std::string &newData = GENESIS_DATA);

    ~blockchain();

    void addBlock(const std::string &newDescription, const std::string &newData);

    private:
    size_t m_lastIndex;
    const std::string m_blockchainPath;

    void addGenBlock(const std::string &newDescription, const std::string &newData);
    void writeBlock(const block *blockToWrite);
    
    std::string getHash(size_t index);
    std::string getFile(size_t index);
};

#endif // BLOCKCHAIN_H