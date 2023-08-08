#ifndef TREEITEM_H
#define TREEITEM_H

#include <QString>
#include <QApplication>
#include <QStandardItemModel>


class TreeItem
{
private:
    QString data;
    TreeItem* parent;
    QVector<TreeItem*> children;
    int id;
    bool deleted;
    bool modified;


public:

    TreeItem(); // Конструктор по умолчанию
    TreeItem(const QString& data, TreeItem* parent = nullptr); // Конструктор с параметрами
    TreeItem(const TreeItem& other); //конструктор копирования, но без связей

    void removeItem();


    QString getData() const;
    TreeItem* getParent() const;
    QVector<TreeItem*> getChildren() const;
    int getId() const;

    bool isDeleted() const { return deleted; }
    bool isModified() const { return modified; }

    QStandardItem* getItem() const;
    TreeItem* getTreeItemFromQStandardItem(QStandardItem* item);

    void setModified(bool value) { modified = value; }
    void setDeleted(bool value) { deleted = value; }
    void setData(const QString& newData);
    void addChild(TreeItem* child);
};

#endif // TREEITEM_H
