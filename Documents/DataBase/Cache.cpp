#include "Cache.h"


Cache::Cache() {}

Cache::~Cache() {
    for (TreeItem* item : cacheItems)
    {
        delete item; // Освобождаем память, так как больше не нужны
    }
}

void Cache::addItem(TreeItem* item)
{
    cacheItems.push_back(item);
}


TreeItem* Cache::findItemById(int id) const
{
    for (TreeItem* item : cacheItems)
    {
        if (item->getId() == id)
        {
            return item;
        }
    }
    return nullptr;
}

QVector<TreeItem*> Cache::getAllItems() const
{
    return cacheItems;
}

QVector<int> Cache::getAddItemsId() const
{
    return addItemsId;
}

void Cache::setAddItemId(int id) {
    addItemsId.push_back(id);
}

void Cache::clear() {
    cacheItems.clear();
    addItemsId.clear();
}

TreeItem* Cache::getTreeItemFromQStandardItem(QStandardItem* item)
{
    if (!item) return nullptr;

    int id = item->data(Qt::UserRole).toInt(); // Получаем ID из пользовательской роли

    for (TreeItem* Item : cacheItems)
    {
        if (Item->getId() == id)
        {
            return Item;
        }
    }
    return nullptr;
}
