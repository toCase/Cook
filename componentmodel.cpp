#include "componentmodel.h"

ComponentModel::ComponentModel(QObject *parent)
    : QAbstractTableModel{parent}
{
}

int ComponentModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _data.count();
}

int ComponentModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 7;
}

QVariant ComponentModel::data(const QModelIndex &index, int role) const
{
    QVariant res;
    if (role == Qt::DisplayRole) {
        int row = index.row();
        int col = index.column();
        QVariantMap map = _data.value(row);
        if (col == 0) {
            res = map.value("id");
        }
        if (col == 1) {
            res = map.value("name");
        }
        if (col == 2) {
            res = map.value("set");
        }
        if (col == 3) {
            res = map.value("act");
        }
        if (col == 4) {
            res = map.value("water");
        }
        if (col == 5) {
            res = map.value("start");
        }
        if (col == 6) {
            res = map.value("fin");
        }
    }
    return res;
}

QVariant ComponentModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case 0:
            return QVariant(QString::fromUtf8("id"));
        case 1:
            return QVariant(QString::fromUtf8("Название"));
        case 2:
            return QVariant(QString::fromUtf8("Set, кг"));
        case 3:
            return QVariant(QString::fromUtf8("Act, кг"));
        case 4:
            return QVariant(QString::fromUtf8("Пром. вод, кг"));
        case 5:
            return QVariant(QString::fromUtf8("Начало загр"));
        case 6:
            return QVariant(QString::fromUtf8("Оконч загр"));
        default:
            return QVariant();
        }
    }
    if (orientation == Qt::Vertical) {
        return QVariant(section + 1);
    }
    return QVariant();
}

void ComponentModel::loadModel(int recipe)
{
    emit beginResetModel();
    _data.clear();
    _data = db.components(recipe);
    emit endResetModel();
}
