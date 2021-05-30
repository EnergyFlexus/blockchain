#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <ctime>
#include <sstream>
#include <fstream>

#define PUBLIC_KEY "publicKey=\n"
#define INDEX "index=\n"
#define BIND_HASH "bindHash=\n"
#define SIGN "sign=\n"
#define DATA "data=\n"

//  publicKey   //
//    index     //
//   bindHash   //
//     sign     //
//     data     //

class block
{
    public:

    block           () = default;
    block           (const block&);
    explicit block  (const std::string&); // get block from string

    ~block          () = default;

    /* getters */
    std::string publicKey   () const;
    size_t index            () const;
    std::string bindHash    () const;
    std::string sign        () const;
    std::string data        () const;

    /* setters */
    void setPublicKey   (const std::string&);
    void setIndex       (const size_t);
    void setBindHash    (const std::string&);
    void setSign        (const std::string&);
    void setData        (const std::string&);

    /* work with strings */
    std::string toString    () const;
    void fromString         (const std::string&);

    private:
    std::string m_publicKey;
    size_t m_index;
    std::string m_bindHash;
    std::string m_sign;
    std::string m_data;
};

#endif // BLOCK_H