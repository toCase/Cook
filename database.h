#pragma once

#include <QObject>
#include <QtSql/QtSql>
#include <QDateTime>


class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);    

signals:

public slots:
    void connectDB();
    void disconnectDB();

    QVariantMap recipeSave(QVariantMap card);
    QMap<int, QVariantMap> recipes(QList<qint64> filter);
    QString recipeDelete(int recipe);

    QVariantMap componentSave(QVariantMap card);
    QMap<int, QVariantMap> components(int recipe);
};
