#include "mainwindow.h"
#include <iostream>
#include <QApplication>
#include <QString>
#include <QTreeView>
#include <QStandardItemModel>
#include <QPushButton>
#include "TreeItem.h"
#include "DataBase.h"
#include "Cache.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    return a.exec();
}
