#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QDebug>
#include <QString>
#include <QTreeView>
#include <QStandardItemModel>
#include <QPushButton>
#include "TreeItem.h"
#include "DataBase.h"
#include "Cache.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QTreeView;
class QStandardItemModel;
class TreeItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void onTreeViewClicked(const QModelIndex &index);
    void onCacheTreeViewClicked(const QModelIndex &index);
    void addAllItemsFromArrayToTreeView(QStandardItemModel* model, QVector<TreeItem*> allItems);
    void addAllItemsToTreeView(TreeItem* currentItem, QStandardItem* parentItem);
    ~MainWindow();

private slots:
    void onCacheUpdateButtonClicked();
    void onAddButtonClicked();
    void onCacheAddButtonClicked();
    void onCacheDeleteButtonClicked();
    void onApplyAllChangesButtonClicked();
    void onRefreshButtonClicked();

private:
    Ui::MainWindow* ui;
    QStandardItemModel *model;
    QStandardItemModel *modelCache;
    QStandardItem* currentSelectedItem = nullptr;  //текущий выбранный элемент из treeView
    QStandardItem* currentSelectedCacheItem = nullptr;
    Database* database;
    Cache* cache;
    TreeItem* root;

    friend void start(MainWindow &mainWindow);
};
#endif // MAINWINDOW_H
