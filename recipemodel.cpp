
#include "recipemodel.h"

RecipeModel::RecipeModel(QObject *parent)
    : QAbstractTableModel{parent}
{

}

int RecipeModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _data.count();
}

int RecipeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 4;
}

QVariant RecipeModel::data(const QModelIndex &index, int role) const
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
            QDateTime ds = QDateTime::fromSecsSinceEpoch(map.value("start").toInt());
            res = QVariant(ds.toString("hh:mm dd:MM:yy"));
        }
        if (col == 3) {
            QDateTime df = QDateTime::fromSecsSinceEpoch(map.value("fin").toInt());
            res = QVariant(df.toString("hh:mm dd:MM:yy"));
        }
    }
    return res;
}

QVariant RecipeModel::headerData(int section, Qt::Orientation orientation, int role) const
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
            return QVariant(QString::fromUtf8("Название рецепта"));
        case 2:
            return QVariant(QString::fromUtf8("Дата начала дозирования"));
        case 3:
            return QVariant(QString::fromUtf8("Дата конца"));
        default:
            return QVariant();
        }
    }
    if (orientation == Qt::Vertical) {
        return QVariant(section + 1);
    }
    return QVariant();
}

QHash<int, QByteArray> RecipeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ID] = "id";
    roles[Name] = "name";
    roles[Start] = "start";
    roles[Fin] = "fin";
    return roles;
}

void RecipeModel::loadModel(QList<qint64> filter)
{
    emit beginResetModel();
    _data.clear();
    _data = db.recipes(filter);
    emit endResetModel();
}

QVariant RecipeModel::get(QString field, const QModelIndex &index)
{
    QVariantMap map = _data.value(index.row());
    return map.value(field);
}
