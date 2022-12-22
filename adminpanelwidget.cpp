#include "passwordenter.h"
#include "adminpanelwidget.h"
#include "ui_adminpanelwidget.h"

class ReplaceDelegate: public QStyledItemDelegate {
private:
    int mode;
public:
    ReplaceDelegate(int mode)
    {
        this->mode = mode;
    }
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index)
        const Q_DECL_OVERRIDE
    {
        if (index.column() == this->mode) {
            int value = index.model()->data(index, Qt::DisplayRole).toInt();
            QString str;
            switch (value) {
            case 1:
                str = " уровень «общий»";
                break;
            case 2:
                str = " уровень «для служебного пользования»";
                break;
            case 3:
                str = " уровень «конфиденциально»";
                break;
            case 4:
                str = " уровень «администратор»";
                break;
            }
            if (option.state.testFlag (QStyle::State_Selected)) {
                painter->fillRect(option.rect, option.palette.highlight());
                qApp->style()->drawItemText(painter, option.rect, option.displayAlignment, option.palette, true, str, QPalette::HighlightedText);
            } else {
                painter->drawText(option.rect, option.displayAlignment, str);
            }
        } else {
            return QStyledItemDelegate::paint(painter, option, index);
        }
    }

    QSize sizeHint (const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE
    {
        if (index.column() == 0)
            return QSize(40, 82);
        else
            return QSize(250, 82);
    }


};

AdminPanelWidget::AdminPanelWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminPanelWidget)
{
    ui->setupUi(this);
    //userDB
    dataDB = QSqlDatabase::addDatabase("QSQLITE", "DATA2");
    QString pathToDB = QDir::currentPath() + QString("/db/data.sqlite");
    dataDB.setDatabaseName(pathToDB);
    dataDB.open();
    dataModel = new QSqlTableModel(this, dataDB);
    dataModel->setTable("files");
    dataModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    dataModel->select();
    dataModel->setHeaderData(0, Qt::Horizontal, "№");
    dataModel->setHeaderData(1, Qt::Horizontal, "Путь до файла");
    dataModel->setHeaderData(2, Qt::Horizontal, "Значение MD5");
    dataModel->setHeaderData(3, Qt::Horizontal, "Кем редактирован");
    dataModel->setHeaderData(4, Qt::Horizontal, "Дата редактирования");
    dataModel->setHeaderData(5, Qt::Horizontal, "Уровень доступа");
    ui->tableView_2->setItemDelegate(new ReplaceDelegate(5));
    ui->tableView_2->setModel(dataModel);
    ui->tableView_2->setColumnHidden(0, true);
    ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    passDB = QSqlDatabase::addDatabase("QSQLITE", "PASS2");
    QString pathToDB2 = QDir::currentPath() + QString("/db/pass.sqlite");
    passDB.setDatabaseName(pathToDB2);
    passDB.open();
    dataModel2 = new QSqlTableModel(this, passDB);
    dataModel2->setTable("PASSWORDS");
    dataModel2->setEditStrategy(QSqlTableModel::OnManualSubmit);
    dataModel2->select();
    dataModel2->setHeaderData(0, Qt::Horizontal, "Пользователь");
    dataModel2->setHeaderData(2, Qt::Horizontal, "Уровень доступа");

    ui->tableView_3->setItemDelegate(new ReplaceDelegate(2));
    ui->tableView_3->setModel(dataModel2);
    ui->tableView_3->setColumnHidden(1, true);
    ui->tableView_3->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView_3->resizeColumnsToContents();
}

AdminPanelWidget::~AdminPanelWidget()
{
    delete ui;
}

void AdminPanelWidget::set_new_pass(QString p)
{
    this->new_pass = p;
}


void AdminPanelWidget::on_writeButton_clicked()
{
    dataModel->submitAll();
    LOG(INFO) << QString("Admin edited data database").toStdString();
}


void AdminPanelWidget::on_deleteButton_clicked()
{
    QItemSelectionModel *select = ui->tableView_2->selectionModel();
    if (select->hasSelection())
    {
        QModelIndexList s = select->selectedRows();
        for (QModelIndex &item : s)
        {
            LOG(INFO) << QString("Admin deleted file " + dataModel->data(item.siblingAtColumn(1)).toString()).toStdString();
            dataModel->removeRows(item.row(), 1);
        }
        dataModel->submitAll();
        QSqlQuery qry;
        qry = dataDB.exec("UPDATE `sqlite_sequence` SET `seq` = (SELECT MAX(`id`) FROM \'files\') WHERE `name` = \'files\';");

    }
}


void AdminPanelWidget::on_pushButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    QString fileName;
    if (dialog.exec())
        fileName = dialog.selectedFiles().at(0);
    if (!fileName.isEmpty())
    {
        QByteArray data;
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
        data = file.readAll();
        QString hash_pass = QString((QCryptographicHash::hash(data, QCryptographicHash::Md5)).toHex());
        QSqlRecord record = dataModel->record();
        record.remove(record.indexOf("id"));
        record.setValue("path", fileName);
        record.setValue("md5", hash_pass);
        record.setValue("editedBy", "Admin");
        record.setValue("degree", 3);
        QDate cd = QDate::currentDate();
        QTime ct = QTime::currentTime();
        QString timedate = ct.toString("HH:mm") + " " + cd.toString("dd.MM.yyyy");
        record.setValue("editedAt", timedate);
        LOG(INFO) << QString("Admin added file " + fileName).toStdString();
        if(dataModel->insertRecord(-1, record))
        {

                dataModel->submitAll();
        }
    }

}


void AdminPanelWidget::on_changePassButton_clicked()
{
    QItemSelectionModel *select = ui->tableView_3->selectionModel();
    if (select->hasSelection())
    {
        QModelIndexList s = select->selectedRows(1);
        for (QModelIndex &item : s)
        {
            QModelIndex sib = item.siblingAtColumn(0);
            PasswordEnter* p = new PasswordEnter("Смена пароля для пользователя " + dataModel2->data(sib).toString(), this);
            if (p->exec())
            {
                LOG(INFO) << QString("Admin changed password for " + dataModel2->data(sib).toString()).toStdString();
                dataModel2->setData(item, this->new_pass);
            }
        }
        dataModel2->submitAll();
    }
}


void AdminPanelWidget::on_deleteUserButton_clicked()
{
    QItemSelectionModel *select = ui->tableView_3->selectionModel();
    if (select->hasSelection())
    {
        QModelIndexList s = select->selectedRows();
        for (QModelIndex &item : s)
        {
            QModelIndex sib = item.siblingAtColumn(0);
            LOG(INFO) << QString("Admin changed password for " + dataModel2->data(sib).toString()).toStdString();
            dataModel2->removeRows(item.row(), 1);
        }
        dataModel2->submitAll();
    }
}


void AdminPanelWidget::on_addUserButton_clicked()
{
    PasswordEnter* p = new PasswordEnter("Введите пароль для нового пользователя", this);
    p->exec();
    QSqlRecord record = dataModel2->record();
    record.setValue("login", "user");
    record.setValue("pass", this->new_pass);
    record.setValue("degree", 1);
    if(dataModel2->insertRecord(-1, record))
    {
            LOG(INFO) << QString("Admin added new user").toStdString();
            dataModel2->submitAll();
    }
}


void AdminPanelWidget::on_writeButton_2_clicked()
{
    dataModel2->submitAll();
    LOG(INFO) << QString("Admin edited password database").toStdString();
}

