#include "DataBase.h"

#include <iostream>

Database::Database() {}

Database::~Database() {
    for (TreeItem* item : items)
    {
        delete item; // Освобождаем память, так как больше не нужны
    }
}

void Database::addItem(TreeItem* item)
{
    if (item) items.push_back(item);
}

void Database::clear() {
    items.clear();
}


TreeItem* Database::getItemById(int id)
{
    foreach (TreeItem* item, items) {
        //проходим по всем элементам и ищем нужный по id
        if (item->getId() == id) return item;
    }
}


QVector<TreeItem*> Database::getAllItems() const
{
    return items;
}


TreeItem* Database::getTreeItemFromQStandardItem(QStandardItem* item)
{
    if (!item) return nullptr;

    int id = item->data(Qt::UserRole).toInt(); // Получаем ID из пользовательской роли

    for (TreeItem* Item : items)
    {
        if (Item->getId() == id)
        {
            return Item;
        }
    }
    return nullptr;
}
