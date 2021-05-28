#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "block.h"
#include "hash.h"

#include <iostream>
#include <cstdlib>
#include <sys/stat.h>

// extension - расширение
#define EXTENSION ".txt"

// Если еще ничего нет, или же он не нашел генезисный блок, в ластиндексе будет это
#define INDEX_NO_GEN_BLOCK (size_t)(-1)

class blockchain
{
public:

    explicit blockchain(const std::string& _blockchainPath);    // путь до бч                   

    blockchain  () = delete;
    blockchain  (const blockchain&) = delete;
    blockchain  (blockchain&&) = delete;

    ~blockchain() = default;

    /* getters */
    std::string blockchainPath  () const;
    size_t lastIndex            () const;

    // эта штука создает блок и потом вызывает addBlock, который его добавит в бч
    block createBlock(const std::string &_publicKey,        // пб ключ
                const std::string &_privateKey,             // пр ключ
                const std::string &_data);                  // сама инфа

    // делаем генезисный блок
    void createGenBlock(const std::string &_genPublicKey,   // пб ключ (ну надо же с чего-то начинать) чтоб оно валидно было
        const std::string &_genPrivateKey,                  // пр ключ
        const std::string &_genBindHash,                    // любой хэшик (можно просто набор символов)
        const std::string &_genData);                       // ну и чета в ген блоке надо


    // эта штука его проверит и решит, добавлять его или нет
    // 0 - все ок + запишет в файлик; -1 - хэш невалиден; -2 - эцп невалидно; -3 - индекс не тот;
    int addBlock(const block&);

    // эта параша удаляет сколько-то блоков с конца, понадобится если бч окажется невалидной
    void deleteBlocks(const size_t);

    // проверяет валидность блока
    bool isBindHashValid(size_t) const;
    bool isBindHashValid(const block&) const;

    // валидность хешей всего бч, вернет индекс того блока, откуда все невалидно, 0 = все ок
    size_t isBindHashValidAll();

    // проверяет эцп у блока
    bool isSignValid(size_t) const;
    bool isSignValid(const block&) const;

    // получает весь блок(файл)
    block getBlock(size_t) const;

private:
    size_t m_lastIndex;                 // индекс последнего блока(файла), шоб знать какой будет некст
    const std::string m_blockchainPath; // путь от экзешника до папки с блоками(файлами)

    // пишет блок в файл
    void writeBlock(const block*);

    // мб бч уже есть в этой папке? выдаст свежий m_lastIndex
    size_t isAlreadyExist();
};

#endif // BLOCKCHAIN_H
