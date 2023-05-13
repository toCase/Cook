
#include "cook.h"
#include "./ui_cook.h"


Cook::Cook(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Cook)
{
    ui->setupUi(this);

    DB.connectDB();


    ui->calendarWidget->setSelectedDate(QDate::currentDate());

    model = new RecipeModel();
    model->loadModel(getFilterDate(QDate::currentDate()));
    ui->tableView_recipe->setModel(model);
    ui->tableView_recipe->setColumnHidden(0, true);
    ui->tableView_recipe->setColumnHidden(3, true);
    ui->tableView_recipe->setColumnWidth(1, 150);

    modelC = new ComponentModel();
    ui->tableView_component->setModel(modelC);
    ui->tableView_component->setColumnHidden(0, true);
    ui->tableView_component->setColumnWidth(1, 250);
    ui->tableView_component->setColumnWidth(2, 70);
    ui->tableView_component->setColumnWidth(3, 70);

    ui->tableWidget_data->setColumnHidden(0, true);
    ui->tableWidget_data->setColumnWidth(0, 12);
    ui->tableWidget_data->setColumnWidth(1, 300);
    ui->tableWidget_data->setColumnWidth(2, 90);
    ui->tableWidget_data->setColumnWidth(3, 90);
    ui->tableWidget_data->setColumnWidth(4, 120);
    ui->tableWidget_data->setColumnWidth(5, 170);
    ui->tableWidget_data->setColumnWidth(6, 120);

    recipeNew();

    connect(ui->spinBox_count, &QSpinBox::valueChanged, this, &Cook::addLine);
    connect(ui->pushButton_save, &QPushButton::clicked, this, &Cook::recipeSave);
    connect(ui->calendarWidget, &QCalendarWidget::clicked, this, &Cook::recipeRead);

    connect(ui->tableView_recipe, &QTableView::clicked, this, &Cook::componentRead);
    connect(ui->tableView_recipe, &QTableView::doubleClicked, this, &Cook::recipeEdit);

    connect(ui->pushButton_new, &QPushButton::clicked, this, &Cook::recipeNew);
    connect(ui->pushButton_del, &QPushButton::clicked, this, &Cook::recipeDelete);
}

Cook::~Cook()
{
    DB.disconnectDB();
    delete ui;
}

void Cook::recipeSave()
{
    QVariantMap card;
    card.insert("id", ID);
    card.insert("name", ui->lineEdit_name->text());
    card.insert("start", ui->dateTimeEdit_start->dateTime().toSecsSinceEpoch());
    card.insert("fin", ui->dateTimeEdit_fin->dateTime().toSecsSinceEpoch());

    QVariantMap map = DB.recipeSave(card);
    if (!map.value("err").toString().isEmpty()){
        ui->statusbar->showMessage(map.value("err").toString(), 5000);
    } else {
        ID = map.value("id").toInt();
        if (ui->tableWidget_data->rowCount() > 0){
            for (int r = 0; r < ui->tableWidget_data->rowCount(); r++){
                QVariantMap cardC;
                cardC.insert("id", ui->tableWidget_data->item(r, 0)->text());
                for (int c = 1; c < 7; c++){
                    QLineEdit *l = dynamic_cast<QLineEdit*>(ui->tableWidget_data->cellWidget(r, c));
                    switch (c) {
                    case 1:
                        cardC.insert("name", l->text());
                    case 2:
                        cardC.insert("set", l->text());
                    case 3:
                        cardC.insert("act", l->text());
                    case 4:
                        cardC.insert("water", l->text());
                    case 5:
                        cardC.insert("start", l->text());
                    case 6:
                        cardC.insert("fin", l->text());
                    default:
                        break;
                    }
                }
                cardC.insert("recipe", ID);
                QVariantMap mapC = DB.componentSave(cardC);
                if (mapC.value("err").toString().isEmpty()){
                    ui->tableWidget_data->item(r, 0)->setText(mapC.value("id").toString());
                    ui->statusbar->showMessage("Сохранено", 2000);
                } else {
                    ui->statusbar->showMessage(mapC.value("err").toString(), 5000);
                }
            }
        }
        model->loadModel(getFilterDate(ui->calendarWidget->selectedDate()));
    }
}

void Cook::recipeRead(QDate _date)
{
    model->loadModel(getFilterDate(_date));
}

void Cook::addLine(int row)
{
    QDoubleValidator *vDouble = new QDoubleValidator();
    vDouble->setDecimals(3);
    vDouble->setBottom(0.00);

    QRegularExpressionValidator *valTime = new QRegularExpressionValidator();
    QRegularExpression expTime("([0-1]?[0-9]|2[0-3]):[0-5][0-9]");
    valTime->setRegularExpression(expTime);

    if (row < ui->tableWidget_data->rowCount()){
        ui->tableWidget_data->removeRow(row);
    } else {

        int _r = row - 1;
        ui->tableWidget_data->insertRow(_r);

        QTableWidgetItem *itemID = new QTableWidgetItem();
        itemID->setText("0");
        ui->tableWidget_data->setItem(_r, 0, itemID);

        QLineEdit *itemName = new QLineEdit();
        itemName->setFrame(false);
        itemName->setPlaceholderText(QString("Component %0").arg(row));
        ui->tableWidget_data->setCellWidget(_r, 1, itemName);

        QLineEdit *itemSet = new QLineEdit();
        itemSet->setValidator(vDouble);
        itemSet->setFrame(false);
        itemSet->setAlignment(Qt::AlignHCenter);
        itemSet->setPlaceholderText("0,00");
        ui->tableWidget_data->setCellWidget(_r, 2, itemSet);

        QLineEdit *itemAct = new QLineEdit();
        itemAct->setValidator(vDouble);
        itemAct->setFrame(false);
        itemAct->setAlignment(Qt::AlignHCenter);
        itemAct->setPlaceholderText("0,00");
        ui->tableWidget_data->setCellWidget(_r, 3, itemAct);

        QLineEdit *itemW = new QLineEdit();
        itemW->setValidator(vDouble);
        itemW->setFrame(false);
        itemW->setAlignment(Qt::AlignHCenter);
        itemW->setPlaceholderText("0,00");
        ui->tableWidget_data->setCellWidget(_r, 4, itemW);

        QLineEdit *itemS = new QLineEdit();
        itemS->setValidator(valTime);
        itemS->setFrame(false);
        itemS->setAlignment(Qt::AlignHCenter);
        itemS->setPlaceholderText(QTime::currentTime().toString("hh:mm"));
        ui->tableWidget_data->setCellWidget(_r, 5, itemS);

        QLineEdit *itemF = new QLineEdit();
        itemF->setValidator(valTime);
        itemF->setFrame(false);
        itemF->setAlignment(Qt::AlignHCenter);
        itemF->setPlaceholderText(QTime::currentTime().toString("hh:mm"));
        ui->tableWidget_data->setCellWidget(_r, 6, itemF);
    }
}

QList<qint64> Cook::getFilterDate(QDate _date)
{
    QList<qint64> filter;

    QDateTime dt_start;
    dt_start.setDate(_date);
    dt_start.setTime(QTime::fromString("00:00", "hh:mm"));
    QDateTime dt_fin;
    dt_fin.setDate(_date);
    dt_fin.setTime(QTime::fromString("23:59", "hh:mm"));

    filter.append(dt_start.toSecsSinceEpoch());
    filter.append(dt_fin.toSecsSinceEpoch());

    return filter;
}

void Cook::componentRead(const QModelIndex &idx)
{
    modelC->loadModel(model->get("id", idx).toInt());
}

void Cook::recipeEdit(const QModelIndex &idx)
{
    ui->tableWidget_data->setRowCount(0);

    QDoubleValidator *vDouble = new QDoubleValidator();
    vDouble->setDecimals(3);
    vDouble->setBottom(0.00);

    QRegularExpressionValidator *valTime = new QRegularExpressionValidator();
    QRegularExpression expTime("([0-1]?[0-9]|2[0-3]):[0-5][0-9]");
    valTime->setRegularExpression(expTime);

    ID = model->get("id", idx).toInt();
    ui->lineEdit_name->setText(model->get("name", idx).toString());
    ui->dateTimeEdit_start->setDateTime(QDateTime::fromSecsSinceEpoch(model->get("start", idx).toInt()));
    ui->dateTimeEdit_fin->setDateTime(QDateTime::fromSecsSinceEpoch(model->get("fin", idx).toInt()));

    QMap<int, QVariantMap> components = DB.components(ID);
    ui->spinBox_count->setValue(components.count());
    for (int r = 0;  r < components.count(); r++){
        QVariantMap map = components.value(r);

        ui->tableWidget_data->insertRow(r);

        QTableWidgetItem *itemID = new QTableWidgetItem();
        itemID->setText(map.value("id").toString());
        ui->tableWidget_data->setItem(r, 0, itemID);

        QLineEdit *itemName = new QLineEdit();
        itemName->setFrame(false);
        itemName->setText(map.value("name").toString());
        ui->tableWidget_data->setCellWidget(r, 1, itemName);

        QLineEdit *itemSet = new QLineEdit();
        itemSet->setValidator(vDouble);
        itemSet->setFrame(false);
        itemSet->setAlignment(Qt::AlignHCenter);
        itemSet->setText(map.value("set").toString());
        ui->tableWidget_data->setCellWidget(r, 2, itemSet);

        QLineEdit *itemAct = new QLineEdit();
        itemAct->setValidator(vDouble);
        itemAct->setFrame(false);
        itemAct->setAlignment(Qt::AlignHCenter);
        itemAct->setText(map.value("act").toString());
        ui->tableWidget_data->setCellWidget(r, 3, itemAct);

        QLineEdit *itemW = new QLineEdit();
        itemW->setValidator(vDouble);
        itemW->setFrame(false);
        itemW->setAlignment(Qt::AlignHCenter);
        itemW->setText(map.value("water").toString());
        ui->tableWidget_data->setCellWidget(r, 4, itemW);

        QLineEdit *itemS = new QLineEdit();
        itemS->setValidator(valTime);
        itemS->setFrame(false);
        itemS->setAlignment(Qt::AlignHCenter);
        itemS->setText(map.value("start").toString());
        ui->tableWidget_data->setCellWidget(r, 5, itemS);

        QLineEdit *itemF = new QLineEdit();
        itemF->setValidator(valTime);
        itemF->setFrame(false);
        itemF->setAlignment(Qt::AlignHCenter);
        itemF->setText(map.value("fin").toString());
        ui->tableWidget_data->setCellWidget(r, 6, itemF);
    }
}

void Cook::recipeNew()
{
    ID = 0;
    ui->spinBox_count->setValue(1);
    ui->lineEdit_name->clear();
    ui->dateTimeEdit_start->setDate(QDate::currentDate());
    ui->dateTimeEdit_start->setTime(QTime::currentTime());
    ui->dateTimeEdit_fin->setDate(QDate::currentDate());
    ui->dateTimeEdit_fin->setTime(QTime::fromString("23:59", "hh:mm"));

    ui->tableWidget_data->setRowCount(0);

    addLine(1);
}

void Cook::recipeDelete()
{
    if (ID > 0) {
        QString err = DB.recipeDelete(ID);
        if (err.isEmpty()){
            model->loadModel(getFilterDate(ui->calendarWidget->selectedDate()));
            ui->statusbar->showMessage("Удалено", 2000);
        recipeNew();
        } else {
            ui->statusbar->showMessage(err, 5000);
        }
    } else {
        ui->statusbar->showMessage("Не выбран рецепт для удаления", 5000);
    }
}


