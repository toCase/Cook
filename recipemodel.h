#pragma once
#include <QAbstractTableModel>
#include <QObject>

#include "database.h"

class RecipeModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit RecipeModel(QObject *parent = nullptr);

    enum ModelRoles {
        ID = Qt::UserRole + 1,
        Name = Qt::UserRole + 2,
        Start = Qt::UserRole + 3,
        Fin = Qt::UserRole +4
    };

    QMap<int, QVariantMap> _data;

    Database db;

public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QHash<int, QByteArray> roleNames() const;
    void loadModel(QList<qint64> filter);
    QVariant get(QString field, const QModelIndex &index);
};
