#include <iostream>
#include "blockchain.h"

#define BLOCKCHAIN_PATH "blocks/"

/* Чтобы все норм работало установи OpenSSL и пропиши его в Path */
/* И еще ручками создай папку пустую, потом мб програмно создавать будем */
/* Да прибудет с вами Бог */

int main(int argc, char **argv)
{
    std::string data = "123";
    std::ifstream fout("myprivate.pem");
    std::string priv;
    priv = streamRead(&fout);
    fout.close();
    fout.open("mypublic.pem");
    std::string pub;
    pub = streamRead(&fout);
    std::string sign;
    sign = hash::rsaSign(priv, data);
    sign = hash::base64Encode(sign);
    std::cout << hash::rsaVerify(pub, hash::base64Decode(sign), data);
    return 0;
}
// чета тут накопировал, пойдет для Сани
/*
Общение с другими узлами
Важной функцией узла является — разделение и синхронизация блокчейн с другими узлами. Правила — 
используемые для поддержания синхронизации сети:

Когда узел генерирует новый блок, он транслирует его в сеть
Когда узел подключается к новой одноранговой сети он опирается на последний блок
Когда узел обнаруживает блок, который имеет индекс больший, чем текущий известный блок, 
он либо добавляет блок в его нынешнем состоянии в свою собственную цепь либо поддерживает для заполнения блокчейна.
*/