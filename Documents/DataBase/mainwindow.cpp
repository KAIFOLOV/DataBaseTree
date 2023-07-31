#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTreeView>
#include <QStandardItemModel>
#include "TreeItem.h"
#include "DataBase.h"
#include "Cache.h"
#include <QInputDialog>
#include <iostream>


TreeItem* findItemByIdInVector(const QVector<TreeItem*>& items, int id)
{
    for (TreeItem* item : items)
    {
        if (item->getId() == id)
        {
            return item;
        }
    }
    return nullptr;
}


void start(MainWindow &mainWindow)
{

    mainWindow.database = new Database;
    mainWindow.cache = new Cache;

    mainWindow.root = new TreeItem("Im");
    TreeItem* Mom = new TreeItem("Mom");
    TreeItem* Dad = new TreeItem("Dad");
    TreeItem* momGrandmother = new TreeItem("Mom_grandmother");
    TreeItem* momGrandfather = new TreeItem("Mom_grandfather");
    TreeItem* dadGrandmother = new TreeItem("Dad_grandmother");
    TreeItem* dadGrandfather = new TreeItem("Dad_grandfather");
    TreeItem* Vasya = new TreeItem("Vasya");
    TreeItem* Egor = new TreeItem("Egor");

    mainWindow.root->addChild(Mom);
    mainWindow.root->addChild(Dad);
    Mom->addChild(momGrandmother);
    Mom->addChild(momGrandfather);
    Dad->addChild(dadGrandmother);
    Dad->addChild(dadGrandfather);
    momGrandmother->addChild(Egor);
    momGrandmother->addChild(Vasya);


    mainWindow.database->addItem(mainWindow.root);
    mainWindow.database->addItem(Mom);
    mainWindow.database->addItem(Dad);
    mainWindow.database->addItem(momGrandmother);
    mainWindow.database->addItem(momGrandfather);
    mainWindow.database->addItem(dadGrandmother);
    mainWindow.database->addItem(dadGrandfather);
    mainWindow.database->addItem(Vasya);
    mainWindow.database->addItem(Egor);

    mainWindow.addAllItemsFromArrayToTreeView(mainWindow.model, mainWindow.database->getAllItems());
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model = new QStandardItemModel(this);     //завели модель
    ui->treeView->setModel(model);            //установили модель в treeView

    modelCache = new QStandardItemModel(this);
    ui->cacheTreeView->setModel(modelCache);


    start(*this);


    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::onTreeViewClicked);
    connect(ui->cacheTreeView, &QTreeView::clicked, this, &MainWindow::onCacheTreeViewClicked);
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::onAddButtonClicked);
    connect(ui->cacheAddButton, &QPushButton::clicked, this, &MainWindow::onCacheAddButtonClicked);
    connect(ui->cacheUpdateButton, &QPushButton::clicked, this, &MainWindow::onCacheUpdateButtonClicked);
    connect(ui->cacheDeleteButton, &QPushButton::clicked, this, &MainWindow::onCacheDeleteButtonClicked);
    connect(ui->applyAllChangesButton, &QPushButton::clicked, this, &MainWindow::onApplyAllChangesButtonClicked);
    connect(ui->refreshButton, &QPushButton::clicked, this, &MainWindow::onRefreshButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//Получение выбранного элемента для дерева БД
void MainWindow::onTreeViewClicked(const QModelIndex& index)
{
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->treeView->model());
    if (model)
    {
        QStandardItem* selectedItem = model->itemFromIndex(index);
        currentSelectedItem = selectedItem;
    }
}


//Получение выбранного элемента для дерева КЭШ
void MainWindow::onCacheTreeViewClicked(const QModelIndex& index)
{
    QStandardItemModel* cacheModel = qobject_cast<QStandardItemModel*>(ui->cacheTreeView->model());
    if (cacheModel)
    {
        QStandardItem* selectedItem = cacheModel->itemFromIndex(index);
        currentSelectedCacheItem = selectedItem;
    }
}


//Добавление элемента из БД в КЭШ с настройкой иерархии
void MainWindow::onCacheAddButtonClicked()
{
    if (currentSelectedItem)
    {
        //да можно вынести в отдельный метод, но я почему-то решил оставить это здесь
        // Проверяем, есть ли элемент с таким же id в cache
        if (cache->getTreeItemFromQStandardItem(currentSelectedItem) != nullptr)
        {
            int id = cache->getTreeItemFromQStandardItem(currentSelectedItem)->getId();
            bool existsInCache = false;
            for (TreeItem* item : cache->getAllItems())
            {
                if (item->getId() == id)
                {
                    existsInCache = true;
                    break;
                }
            }
            // Если элемент уже существует в cache, ничего не делаем
            if (existsInCache)
            {
                return;
            }
        }

        // Создаем новый элемент через конструктор копирования, но без инфы о предках и потомках
        TreeItem* copiedItem = new TreeItem(*root->getTreeItemFromQStandardItem(currentSelectedItem));
        // Добавляем элемент в вектор cache
        cache->addItem(copiedItem);
        //закинули в вектор id добавленных элементов
        cache->setAddItemId(copiedItem->getId());

        for (TreeItem* ItemCache : cache->getAllItems())
        {
            //Проверяем есть ли у добавляемого элемента дети в КЭШ
            TreeItem* ItemParent = findItemByIdInVector(database->getTreeItemFromQStandardItem(currentSelectedItem)->getChildren(), ItemCache->getId());
            if (ItemParent != nullptr)
            {
                int idParent = ItemParent->getId();
                copiedItem->addChild(cache->findItemById(idParent));
            }

            //Проверяем есть ли родительский элемент для добавляемого в КЭШ
            if (database->getTreeItemFromQStandardItem(currentSelectedItem)->getParent() != nullptr)
            {
                if (ItemCache->getId() == database->getTreeItemFromQStandardItem(currentSelectedItem)->getParent()->getId())
                {
                    ItemCache->addChild(copiedItem);
                }
            }
        }
        //Вывод элементов из КЭШ в дерево
        addAllItemsFromArrayToTreeView(modelCache, cache->getAllItems());
    }
}


//Добавление нового элемента в КЭШ
void MainWindow::onAddButtonClicked()
{
    if (currentSelectedCacheItem != nullptr) {
        QString name = QInputDialog::getText(this, tr("Add Item"), tr("Enter item name:"));
        if (!name.isEmpty()) {
            TreeItem* newItem = new TreeItem(name);
            //добавляем новый элемент в cache
            cache->addItem(newItem);
            // Устанавливаем предка для нового элемента
            cache->getTreeItemFromQStandardItem(currentSelectedCacheItem)->addChild(newItem); 
            // Устанавливаем, что предок изменен
            cache->getTreeItemFromQStandardItem(currentSelectedCacheItem)->setModified(true);
            // Устанавливаем, что добавленный элемент изменен
            newItem->setModified(true);

            //parentItem->addChild(newItem);
            addAllItemsFromArrayToTreeView(modelCache, cache->getAllItems());
        }
    }
}


//Изменение поля data для элемента в КЭШ
void MainWindow::onCacheUpdateButtonClicked()
{
    if (currentSelectedCacheItem != nullptr)
    {
        QString name = QInputDialog::getText(this, tr("Add Item"), tr("Enter item name:"));
        if (!name.isEmpty())
        {
            cache->getTreeItemFromQStandardItem(currentSelectedCacheItem)->setData(name);
        }
        addAllItemsFromArrayToTreeView(modelCache, cache->getAllItems());
    }
}

//Кнопка сброса программы
void MainWindow::onRefreshButtonClicked()
{
    database->clear();
    cache->clear();
    model->clear();
    modelCache->clear();

    currentSelectedCacheItem = nullptr;
    currentSelectedItem = nullptr;

    start(*this);
}


//Удаление элемента в КЭШ, не удаляем а ставим метку "удален"
void MainWindow::onCacheDeleteButtonClicked()
{
    if (currentSelectedCacheItem)
    {
        cache->getTreeItemFromQStandardItem(currentSelectedCacheItem)->setDeleted(true);
        addAllItemsFromArrayToTreeView(modelCache, cache->getAllItems());
    }
}


//Сохранение всех изменений из КЭШ в БД
void MainWindow::onApplyAllChangesButtonClicked()
{
    for (TreeItem* ItemCache : cache->getAllItems())
    {
        if (database->getItemById(ItemCache->getId()) == nullptr)
        {
            // Элемента нет в базе данных, добавляем его вместе с его иерархией
            TreeItem* parent = ItemCache->getParent();
            // Добавляем текущий элемент
            database->addItem(ItemCache);

            for (int cacheId : cache->getAddItemsId())
            {
                //Смотрим был ли родитель нового элемента добавлен в КЭШ
                if (parent->getId() == cacheId)
                {
                    database->getItemById(parent->getId())->addChild(ItemCache);
                }
            }
        }
        else
        {
            //Если элемент есть в базе данных, то проверим удален он или нет и поменяем date
            //тут конечно можно проверять изменилась data или нет
            database->getItemById(ItemCache->getId())->setDeleted(ItemCache->isDeleted());
            database->getItemById(ItemCache->getId())->setData(ItemCache->getData());
        }
    }
    //обновление дерева БД
    addAllItemsFromArrayToTreeView(model, database->getAllItems());
}


//Добавление элементов в treeView вместе с дочерними
void MainWindow::addAllItemsToTreeView(TreeItem* currentItem, QStandardItem* parentItem)
{
    // Создаем QStandardItem для текущего элемента TreeItem
    QStandardItem* item = new QStandardItem(currentItem->getData());
    item->setData(currentItem->getId(), Qt::UserRole);

    // Добавляем созданный элемент в родительский элемент
    parentItem->appendRow(item);

    // Рекурсивно добавляем дочерние элементы текущего элемента TreeItem
    for (TreeItem* child : currentItem->getChildren())
    {
        if (!child->isDeleted()) {
            addAllItemsToTreeView(child, item);
        }
    }
}


//Добавление элементов из вектора в нужную модель
void MainWindow::addAllItemsFromArrayToTreeView(QStandardItemModel* model, QVector<TreeItem*> allItems)
{
    //мысль, идем по массиву и ищем родителя, если встречаем элемент без родителя то добавляем его и всех дочерних

    // Очищаем treeView перед добавлением новых элементов
    model->clear();
    // Получаем все элементы из cache
    int flag = 0;
    for (TreeItem* item : allItems)
    {
        if (!item->isDeleted())
        {
            for (TreeItem* item_parent : allItems)
            {
                if (item->getParent() != nullptr)
                {
                    if (item->getParent()->getId() == item_parent->getId())
                    {
                        flag = 1;
                    }
                }
            }
            if (flag == 0)
            {
                QStandardItem* rootItem = new QStandardItem(item->getData());
                rootItem->setData(item->getId(), Qt::UserRole);

                // Добавляем все дочерние элементы из cache в cacheTreeView
                for (TreeItem* child : item->getChildren())
                {
                    if (!child->isDeleted())
                    {
                        addAllItemsToTreeView(child, rootItem);
                    }
                }
                model->appendRow(rootItem);
            }
        }
        flag = 0;
    }
}
