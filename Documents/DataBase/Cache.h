#ifndef CACHE_H
#define CACHE_H
#include "TreeItem.h"
#include <QObject>
#include <QVector>
#include <iostream>

class Cache : public QObject
{

private:
    QVector<TreeItem*> cacheItems;
    //вектор для хранения id элементов, которые попадают в КЭШ
    QVector<int> addItemsId;

public:
    Cache();
    ~Cache();

    void addItem(TreeItem* item);
    void setAddItemId(int id);
    void clear();

    TreeItem* findItemById(int id) const;
    QVector<TreeItem*> getAllItems() const;
    QVector<int> getAddItemsId() const;

    TreeItem* getTreeItemFromQStandardItem(QStandardItem* item);
};


#endif // CACHE_H
