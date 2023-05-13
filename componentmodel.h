#pragma once
#include <QAbstractTableModel>
#include <QObject>

#include "database.h"

class ComponentModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ComponentModel(QObject *parent = nullptr);

    QMap<int, QVariantMap> _data;

    Database db;

public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void loadModel(int recipe);
};
