#include <QApplication>
#include <QTextCodec>

#include "Client.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
#if _MSC_VER
    QTextCodec* codec = QTextCodec::codecForName("GBK");
#else
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
#endif
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCString(codec);
    QTextCodec::setCodecForTr(codec)
#else
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#endif

    Client client;
    client.show();

    return a.exec();
}
