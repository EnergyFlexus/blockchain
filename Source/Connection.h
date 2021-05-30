#ifndef CONNECTION_H
#define CONNECTION_H

#include "blockchain.h"

#include <QObject>
#include <QUdpSocket>
#include <QDataStream>
#include <QLabel>
#include <QTextEdit>
#include <QTextLine>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTimer>

#define LABEL_PRIVATE_KEY "Path to private Key:"
#define LABEL_PUBLIC_KEY "Path to public Key:"
#define LABEL_FILENAME "Path to file:"

#define BUTTON_OK "Ok"
#define BUTTON_UPDATE "Update"

#define SIGNATURE_ADD_BLOCK "ADD_BLOCK\n"
#define SIGNATURE_LAST_INDEX "LAST_INDEX\n"
#define SIGNATURE_NEED_BLOCK "NEED_BLOCK\n"
#define SIGNATURE_NEW_INDEX "NEW_INDEX\n"

#define BLOCKCHAIN_PATH "blocks/"

#define DELAY_BLOCK_RECIVE 2000

class Connection : public QWidget
{
    Q_OBJECT

private:
    int m_port;
    bool m_update = false;

    blockchain *m_blockchain;
    QUdpSocket *m_socket;
    QTimer *m_timer;

    // GUI
    QVBoxLayout *m_layout;

    QLabel *m_labelPrivateKey;
    QLabel *m_labelPublicKey;
    QLabel *m_labelFilename;

    QLineEdit *m_linePrivateKey;
    QLineEdit *m_linePublicKey;
    QLineEdit *m_lineFilename;

    QTextEdit *m_textInfo;

    QPushButton *m_buttonOk;
    QPushButton *m_buttonUpdate;

    // signatures
    const QString s_add_block = SIGNATURE_ADD_BLOCK;
    const QString s_last_index = SIGNATURE_LAST_INDEX;
    const QString s_need_block = SIGNATURE_NEED_BLOCK;
    const QString s_new_index = SIGNATURE_NEW_INDEX;

    // add info
    void addInfo(const QString &_info);

    // добавляет блок
    void addBlock(const block &_block);

    // пишет в сокет инфо
    void writeDatagram(const QString &_data);

public:
    Connection(int _port, QWidget *_p = 0);
    ~Connection();

private slots:
    // для чтения с сокета
    void slotReadDatagrams();

    // эта штука будет запрашивать новые индексы
    void slotTakeNewIndex();

public slots:
    // кнопки
    void slotButtonOkClicked();
    void slotButtonUpdateClicked();

};

#endif // CONNECTION_H
