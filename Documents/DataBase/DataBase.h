#ifndef DATABASE_H
#define DATABASE_H
#include "TreeItem.h"

class Database {

private:
    QVector<TreeItem*> items;

public:
    Database();
    ~Database();

    void addItem(TreeItem* item);
    void clear();

    TreeItem* getTreeItemFromQStandardItem(QStandardItem* item);
    TreeItem* getItem(TreeItem* item);
    TreeItem* getItemById(int id);

    QVector<TreeItem*> getAllItems() const;

};

#endif // DATABASE_H
