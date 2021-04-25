#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "block.h"
#include "hash.h"

// genesis block - первый блок в блокчейне
// defines for genesis block
// extension - расширение
#define GENESIS_DESC "genesis block"
#define GENESIS_DATA "Hello, blockchain!"
#define EXTENSION ".txt"

/* TODO: checking hashes, checking existing chain, ... */

class blockchain
{
public:
    // у бч ток один конструктор, остальные в парашу
    blockchain(const std::string &_blockchainPath, 
        const std::string &_description = GENESIS_DESC, 
        const std::string &_data = GENESIS_DATA);

    blockchain() = delete;
    blockchain(const blockchain&) = delete;
    blockchain(blockchain&&) = delete;

    ~blockchain();

    /* getters */
    std::string blockchainPath() const;
    size_t lastIndex() const;

    // эта параша добавляет блок очевидно
    void addBlock(const std::string&, const std::string&);

    // эта параша удаляет сколько-то блоков с конца, понадобится если бч окажется невалидной
    void deleteBlocks(const size_t);

private:
    // индекс последнего блока(файла), шоб знать какой будет некст
    size_t m_lastIndex;

    // путь от экзешника до папки с блоками(файлами)
    const std::string m_blockchainPath;

    // добавляет генезисный блок (т.е самый первый в бч)
    void addGenBlock(const std::string& , const std::string&);

    // пишет блок в файл
    void writeBlock(const block*);
    
    // получает хэш из блока(файла)
    std::string getHash(size_t);

    // получает весь блок(файл)
    std::string getFile(size_t);
};

#endif // BLOCKCHAIN_H