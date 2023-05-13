#include "database.h"

Database::Database(QObject *parent)
    : QObject{parent}
{

}

void Database::connectDB() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "db");
    db.setDatabaseName("cook.db3");
    db.open();

    qDebug() << db.isOpen();
}

void Database::disconnectDB() {
    QSqlDatabase db = QSqlDatabase::database("db");
    if (db.isOpen()) {
        db.close();
    }
}

QVariantMap Database::recipeSave(QVariantMap card)
{
    QVariantMap map;
    QSqlDatabase db = QSqlDatabase::database("db");
    if (db.isOpen()) {
        int id = card.value("id").toInt();
        if (id == 0) {
            QString req = "INSERT INTO `recipe` (`name`, `start`, `fin`) VALUES (?, ?, ?)";
            QSqlQuery query(req, db);
            query.bindValue(0, card.value("name").toString());
            query.bindValue(1, card.value("start").toInt());
            query.bindValue(2, card.value("fin").toInt());
            query.exec();

            if (query.lastError().text().isEmpty()){
                id = query.lastInsertId().toInt();
            }
            map.insert("id", id);
            map.insert("err", query.lastError().text());
        } else if (id > 0) {
            QString req = QString("UPDATE `recipe` SET `name` = \'%0\', `start` = \'%1\', fin = \'%2\' "
                                  "WHERE `recipe`.`id` = \'%3\' ")
                              .arg(card.value("name").toString())
                              .arg(card.value("start").toInt())
                              .arg(card.value("fin").toInt())
                              .arg(id);
            QSqlQuery query(req, db);
            query.exec();

            map.insert("id", id);
            map.insert("err", query.lastError().text());
        }
    }
    return map;
}



QMap<int, QVariantMap> Database::recipes(QList<qint64> filter)
{
    QMap<int, QVariantMap> MAP;
    QSqlDatabase db = QSqlDatabase::database("db");
    if (db.isOpen()) {
        int x = 0;
        QString req = QString("SELECT `id`, `name`, `start`, `fin` "
                        "FROM `recipe` "
                        "WHERE (`recipe`.`start` BETWEEN \'%0\' AND \'%1\') "
                              "ORDER BY `recipe`.`start` ")
                          .arg(filter.at(0))
                          .arg(filter.at(1));
        QSqlQuery query(req, db);
        while (query.next()) {
            QVariantMap map;
            map.insert("id", query.value(0).toInt());
            map.insert("name", query.value(1).toString());
            map.insert("start", query.value(2).toInt());
            map.insert("fin", query.value(3).toInt());
            MAP.insert(x, map);
            x++;
        }
    }
    return MAP;
}

QString Database::recipeDelete(int recipe)
{
    QString err;
    QSqlDatabase db = QSqlDatabase::database("db");
    if (db.isOpen()) {
        QSqlQuery query(QString("DELETE FROM `component` WHERE `component`.`recipe` = \'%0\'").arg(recipe), db);
        query.exec();
        err = query.lastError().text();
        if (err.isEmpty()){
            QSqlQuery queryA(QString("DELETE FROM `recipe` WHERE `recipe`.`id` = \'%0\'").arg(recipe), db);
            queryA.exec();
            err = queryA.lastError().text();
        }
    }
    return err;
}

QVariantMap Database::componentSave(QVariantMap card)
{
    QVariantMap map;
    QSqlDatabase db = QSqlDatabase::database("db");
    if (db.isOpen()) {
        int id = card.value("id").toInt();
        if (id == 0) {
            QString req = "INSERT INTO `component` (`cname`, `cset`, `cact`, `cwater`, `cstart`, `cfin`, `recipe`) "
                          "VALUES (?, ?, ?, ?, ?, ?, ?)";
            QSqlQuery query(req, db);
            query.bindValue(0, card.value("name").toString());
            query.bindValue(1, card.value("set").toString());
            query.bindValue(2, card.value("act").toString());
            query.bindValue(3, card.value("water").toString());
            query.bindValue(4, card.value("start").toString());
            query.bindValue(5, card.value("fin").toString());
            query.bindValue(6, card.value("recipe").toString());
            query.exec();

            if (query.lastError().text().isEmpty()){
                id = query.lastInsertId().toInt();
            }
            map.insert("id", id);
            map.insert("err", query.lastError().text());
        } else if (id > 0) {
            QString req = QString("UPDATE `component` SET `cname` = \'%0\', `cset` = \'%1\', cact = \'%2\', "
                                  "`cwater` = \'%3\', `cstart` = \'%4\', `cfin` = \'%5\' "
                                  "WHERE `component`.`id` = \'%6\' ")
                              .arg(card.value("name").toString())
                              .arg(card.value("set").toString())
                              .arg(card.value("act").toString())
                              .arg(card.value("water").toString())
                              .arg(card.value("start").toString())
                              .arg(card.value("fin").toString())
                              .arg(id);
            QSqlQuery query(req, db);
            query.exec();

            map.insert("id", id);
            map.insert("err", query.lastError().text());
        }
    }
    return map;
}

QMap<int, QVariantMap> Database::components(int recipe)
{
    QMap<int, QVariantMap> MAP;
    QSqlDatabase db = QSqlDatabase::database("db");
    if (db.isOpen()) {
        int x = 0;
        QString req = QString("SELECT `id`, `cname`, `cset`, `cact`, `cwater`, `cstart`, `cfin` "
                              "FROM `component` "
                              "WHERE `component`.`recipe` = \'%0\' "
                              "ORDER BY `component`.`id` ")
                          .arg(recipe);
        QSqlQuery query(req, db);
        while (query.next()) {
            QVariantMap map;
            map.insert("id", query.value(0).toInt());
            map.insert("name", query.value(1).toString());
            map.insert("set", query.value(2).toString());
            map.insert("act", query.value(3).toString());
            map.insert("water", query.value(4).toString());
            map.insert("start", query.value(5).toString());
            map.insert("fin", query.value(6).toString());
            MAP.insert(x, map);
            x++;
        }
    }
    return MAP;
}

