#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "block.h"
#include "hash.h"

#include <iostream>
#include <cstdlib>
#include <sys/stat.h>

// extension - расширение

#define EXTENSION ".txt"

class blockchain
{
public:
    // у бч ток один конструктор, остальные в парашу
    blockchain(const std::string &_blockchainPath,  // путь до бч
        const std::string &_genPublicKey,           // пб ключ (ну надо же с чего-то начинать) чтоб оно валидно было
        const std::string &_genPrivateKey,          // пр ключ
        const std::string &_genBindHash,            // любой хэшик (можно просто набор символов)
        const std::string &_genData);               // ну и чета в ген блоке надо

    blockchain  () = delete;
    blockchain  (const blockchain&) = delete;
    blockchain  (blockchain&&) = delete;

    ~blockchain() = default;

    /* getters */
    std::string blockchainPath  () const;
    size_t lastIndex            () const;

    // эта параша создает блок и потом вызывает addBlock, который его добавит в бч
    block createBlock(const std::string &_publicKey,        // пб ключ
                const std::string &_privateKey,             // пр ключ
                const std::string &_data);                  // сама инфа


    // эта штука его проверит и решит, добавлять его или нет
    //   0 - все ок; 1 - хэш в говне; 2 - индекс в говне; -1 - эцп в говне;
    int addBlock(const block&);

    // эта параша удаляет сколько-то блоков с конца, понадобится если бч окажется невалидной
    void deleteBlocks(const size_t);

    // проверяет валидность блока
    bool isBindHashValid(size_t) const;
    bool isBindHashValid(const block&) const;

    // валидность хешей всего бч, вернет индекс того блока, откуда все невалидно, 0 = все ок
    size_t isBindHashValidAll() const;

    // проверяет эцп у блока
    bool isSignValid(size_t) const;
    bool isSignValid(const block&) const;

    // проверяет эцп у всего бч, вернет индекс первого найденного блока с не валдиной эцп, 0 = все ок
    size_t isAllSignValid() const;

    // получает весь блок(файл)
    block getBlock(size_t) const;

private:
    size_t m_lastIndex;                 // индекс последнего блока(файла), шоб знать какой будет некст
    const std::string m_blockchainPath; // путь от экзешника до папки с блоками(файлами)


    // делаем генезисный блок
    void createGenBlock(const std::string &_genPublicKey,
        const std::string &_genPrivateKey,
        const std::string &_genBindHash,
        const std::string &_genData);

    // пишет блок в файл
    void writeBlock(const block*);

    // мб бч уже есть в этой папке? выдаст свежий m_lastIndex
    size_t isAlreadyExist();
};

#endif // BLOCKCHAIN_H