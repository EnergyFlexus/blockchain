#include "Connection.h"

Connection::Connection(int _port, QWidget *_p) : QWidget(_p), m_port(_port)
{
    m_blockchain = new blockchain(BLOCKCHAIN_PATH);

    // GUI
    m_layout = new QVBoxLayout(this);

    m_labelPrivateKey = new QLabel(LABEL_PRIVATE_KEY);
    m_labelPublicKey = new QLabel(LABEL_PUBLIC_KEY);
    m_labelFilename = new QLabel(LABEL_FILENAME);

    m_linePrivateKey = new QLineEdit(this);
    m_linePublicKey = new QLineEdit(this);
    m_lineFilename = new QLineEdit(this);

    m_textInfo = new QTextEdit(this);
    m_textInfo->setReadOnly(true);

    m_buttonOk = new QPushButton(BUTTON_OK, this);
    m_buttonUpdate = new QPushButton(BUTTON_UPDATE, this);

    m_layout->addWidget(m_labelPrivateKey);
    m_layout->addWidget(m_linePrivateKey);

    m_layout->addWidget(m_labelPublicKey);
    m_layout->addWidget(m_linePublicKey);

    m_layout->addWidget(m_labelFilename);
    m_layout->addWidget(m_lineFilename);

    m_layout->addWidget(m_textInfo);

    m_layout->addWidget(m_buttonOk);
    m_layout->addWidget(m_buttonUpdate);

    this->setLayout(m_layout);

    connect(m_buttonOk, SIGNAL(clicked()), this, SLOT(slotButtonOkClicked()));
    connect(m_buttonUpdate, SIGNAL(clicked()), this, SLOT(slotButtonUpdateClicked()));

    // таймер для того чтобы по очереди выкачивать блоки
    m_timer = new QTimer(this);
    m_timer->setInterval(DELAY_BLOCK_RECIVE);

    // sockets
    m_socket = new QUdpSocket(this);
    m_socket->bind(QHostAddress::Any, m_port, QUdpSocket::ReuseAddressHint|QUdpSocket::ShareAddress);
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(slotReadDatagrams()));

    // ну и проверочку после запуска
    this->slotButtonUpdateClicked();
}
Connection::~Connection()
{
    delete m_blockchain;
}
void Connection::addInfo(const QString &_info)
{
    m_textInfo->append(_info);
}
void Connection::addBlock(const block &_block)
{
    // 0 - все ок + запишет в файлик; -1 - хэш невалиден; -2 - эцп невалидно; -3 - индекс не тот;
    int err_code = m_blockchain->addBlock(_block);
    if(err_code == 0) this->addInfo("Block added: " + QString::fromStdString(std::to_string(_block.index())));

}
void Connection::slotButtonOkClicked()
{
    // перед тем как создавать блок чекнем че там
    this->slotButtonUpdateClicked();

    // считываем из файликов
    std::string f_private_key = m_linePrivateKey->text().toStdString();
    std::string f_public_key = m_linePublicKey->text().toStdString();
    std::string f_data = m_lineFilename->text().toStdString();

    std::string private_key;
    std::string public_key;
    std::string data;

    block new_block;

    std::ifstream fin;

    fin.open(f_private_key);
    if(!fin.is_open())
    {
        this->addInfo("Unable to open " + QString::fromStdString(f_private_key));
        return;
    }
    private_key = streamRead(&fin);
    fin.close();

    fin.open(f_public_key);
    if(!fin.is_open())
    {
        this->addInfo("Unable to open " + QString::fromStdString(f_public_key));
        return;
    }
    public_key = streamRead(&fin);
    fin.close();

    fin.open(f_data);
    if(!fin.is_open())
    {
        this->addInfo("Unable to open " + QString::fromStdString(f_data));
        return;
    }
    data = streamRead(&fin);
    fin.close();

    // создаем блок
    new_block = m_blockchain->createBlock(public_key, private_key, data);
    this->addInfo("Block created: " + QString::fromStdString(std::to_string(new_block.index())));

    // обязательно оповестим о добавлении наших друзей
    this->writeDatagram(s_add_block + QString::fromStdString(new_block.toString()));

    // добавляем
    this->addBlock(new_block);
}
void Connection::writeDatagram(const QString &_data)
{
    QByteArray ba_datagram;
    QDataStream out(&ba_datagram, QIODevice::WriteOnly);
    out << _data;
    m_socket->writeDatagram(ba_datagram, QHostAddress::Broadcast, m_port);
    qDebug() << "Sended - " << _data;
}
void Connection::slotReadDatagrams()
{
    // аккуратно считываем данные
    QByteArray ba_datagram;
    do
    {
        ba_datagram.resize(m_socket->pendingDatagramSize());
        m_socket->readDatagram(ba_datagram.data(), ba_datagram.size());
    }
    while(m_socket->hasPendingDatagrams());

    // все данные в str
    QDataStream in(&ba_datagram, QIODevice::ReadOnly);
    QString str;
    in >> str;

    qDebug() <<"Recived - " << str;

    // если нам прислали новый блок - добавляем
    if(str.contains(s_add_block))
    {
        QString substr = str.mid(s_add_block.size(), str.size() - s_add_block.size());
        block new_block;
        new_block.fromString(substr.toStdString());

        this->addBlock(new_block);
    }

    // если у нас спросили какой по номеру последний блок
    if(str.contains(s_last_index))
    {
        this->writeDatagram(s_new_index + QString::fromStdString(std::to_string(m_blockchain->lastIndex())));
    }

    // если у нас попросили какой-то блок
    if(str.contains(s_need_block))
    {
        QString substr = str.mid(s_need_block.size(), str.size() - s_need_block.size());
        int index = substr.toInt();

        // если нас попросили блок которого нет
        if((size_t)index > m_blockchain->lastIndex()) return;
        this->writeDatagram(s_add_block + QString::fromStdString(m_blockchain->getBlock(index).toString()));
    }

    // если нам прислали новый индекс последнего блока, докачиваем то, чего у нас нет
    if(str.contains(s_new_index))
    {
        QString substr = str.mid(s_new_index.size(), str.size() - s_new_index.size());
        int new_index = substr.toInt();

        if(m_blockchain->lastIndex() < (size_t)new_index)
        {
            this->writeDatagram(s_need_block + QString::fromStdString(std::to_string(m_blockchain->lastIndex() + 1)));

            m_timer->start();
            disconnect(m_timer, SIGNAL(timeout()), this, SLOT(slotTakeNewIndex()));
            connect(m_timer, SIGNAL(timeout()), this, SLOT(slotTakeNewIndex()));
        }
        else
        {
            m_timer->stop();
            disconnect(m_timer, SIGNAL(timeout()), this, SLOT(slotTakeNewIndex()));
        }
    }
}
void Connection::slotTakeNewIndex()
{
    this->slotButtonUpdateClicked();
}
void Connection::slotButtonUpdateClicked()
{
    this->writeDatagram(s_last_index);

    size_t err_block = m_blockchain->isBindHashValidAll();

    if(err_block)
    {
        this->addInfo("Oops, err in block - " + QString::fromStdString(std::to_string(err_block)));
        m_blockchain->deleteBlocks(m_blockchain->lastIndex() - err_block + 1);
        if(!m_blockchain->isSignValid(m_blockchain->lastIndex())) m_blockchain->deleteBlocks(1);
        if(m_blockchain->getBlock(m_blockchain->lastIndex()).index() != m_blockchain->lastIndex()) m_blockchain->deleteBlocks(1);
        this->addInfo("Now last block is - " + QString::fromStdString(std::to_string(m_blockchain->lastIndex())));
    }
    this->addInfo("Update completed!");
}
