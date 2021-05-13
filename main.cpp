#include <iostream>
#include "blockchain.h"
#include "block.h"

// Эта то где у нас бч хранится
#define BLOCKCHAIN_PATH "blocks/"

/* Чтобы все норм работало установи OpenSSL и пропиши его в Path */
/* И еще ручками создай папки пустые temp и blocks, потом мб програмно создавать будем */
/* myprivate.pem и mypublic.pem - приватный и публичный ключи в форматне pem, если интересно 
   можете свои сгенерить в OpenSSL */

/* Да прибудет с вами Бог */

int main(int argc, char **argv)
{
    std::string priv;
    std::string pub;
    std::ifstream fin;

    fin.open("myprivate.pem");
    priv = streamRead(&fin);
    fin.close();
    fin.open("mypublic.pem");
    pub = streamRead(&fin);
    fin.close();

    blockchain chain(BLOCKCHAIN_PATH);

    // не забываем про ген блок, если его нет вдруг
    if(chain.lastIndex() == INDEX_NO_GEN_BLOCK) chain.createGenBlock(pub, priv, "HiIAmHash", "I AM A GENBLOCK!!!");

    chain.addBlock(chain.createBlock(pub, priv, "hmmm"));
    chain.addBlock(chain.createBlock(pub, priv, "hmmxxx"));

    // если комп слабый и тем более нет ссд, а у тебя норм там блоков, лучше закомменти эти две строки))0)
    // эти костылявые хэширования через файлики фулл тильтовые, 10 блоков - даже у меня секунду висит, хех
    // надо будет потом по нормальному сделать офк

    std::cout << chain.isBindHashValidAll() << std::endl;
    std::cout << chain.isSignValidAll() << std::endl;
    return 0;
}