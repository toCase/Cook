#pragma once

#include <QMainWindow>
#include <QContextMenuEvent>

#include "database.h"
#include "recipemodel.h"
#include "componentmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Cook; }
QT_END_NAMESPACE

class Cook : public QMainWindow

{
    Q_OBJECT

public:
    Cook(QWidget *parent = nullptr);
    ~Cook();

    Database DB;
    int ID;

    RecipeModel *model;
    ComponentModel *modelC;

private:
    Ui::Cook *ui;

public slots:
    void recipeSave();
    void recipeRead(QDate _date);

    void addLine(int row);

    QList<qint64> getFilterDate(QDate _date);

    void componentRead(const QModelIndex &idx);
    void recipeEdit(const QModelIndex &idx);

    void recipeNew();
    void recipeDelete();

};

