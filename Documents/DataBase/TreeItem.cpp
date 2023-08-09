#include "TreeItem.h"
#include <iostream>

static int idCounter = 0;

TreeItem::TreeItem() : data(""), parent(nullptr), deleted(false), modified(false)
{
    id = idCounter++;
}


TreeItem::TreeItem(const QString& data, TreeItem* parent) : data(data), parent(parent), deleted(false), modified(false)
{
    id = idCounter++;
}

TreeItem::TreeItem(const TreeItem& other)
    : data(other.data),
      parent(nullptr),
      id(other.id),
      deleted(other.deleted),
      modified(other.modified)
{}


void TreeItem::removeItem()
{
    if (deleted == false)
    {
        deleted = true;
        data = data + " (deleted)";
        for (TreeItem* childrenItem : children)
        {
            childrenItem->removeItem();
        }
    }
}




QString TreeItem::TreeItem::getData() const
{
    return data;
}

int TreeItem::TreeItem::getId() const
{
    return id;
}

TreeItem* TreeItem::TreeItem::getParent() const
{
    return parent;
}


QVector<TreeItem*> TreeItem::TreeItem::getChildren() const
{
    return children;
}


void TreeItem::TreeItem::setData(const QString& newData)
{
    data = newData;
}


void TreeItem::TreeItem::addChild(TreeItem* child)
{
    if (child)
    {
        children.push_back(child);
        child->parent = this;
    }
}

//создаем из TreeItem элемент QStandardItem
QStandardItem* TreeItem::getItem() const
{
    QStandardItem* item = new QStandardItem(data);
    item->setData(id, Qt::UserRole); // Сохраняем ID в пользовательском роли
    return item;
}


TreeItem* TreeItem::getTreeItemFromQStandardItem(QStandardItem* item)
{
    if (!item) return nullptr;

    int id = item->data(Qt::UserRole).toInt(); // Получаем ID из пользовательской роли

    if (this->id == id)
        return this;

     //Если ID не совпадает, проверяем дочерние элементы рекурсивно
    for (TreeItem* child : children)
    {
        TreeItem* foundItem = child->getTreeItemFromQStandardItem(item);
        if (foundItem)
            return foundItem;
    }
    return nullptr;
}
