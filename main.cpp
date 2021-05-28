#include <QApplication>
#include "Connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Connection con(2323);
    con.show();

    return a.exec();
}
