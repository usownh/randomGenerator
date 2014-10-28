#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settings=new QSettings("config.ini",
                           QSettings::IniFormat);
    QTime t;
    t= QTime::currentTime();
    qsrand(t.msec()+t.second()*1000);
    tableModel = new QStandardItemModel();
    tableModel->setColumnCount(1);
    tableModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("备选"));
    ui->tableView->setModel(tableModel);
    //表头信息显示居左
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
    ui->tableView->verticalHeader()->setVisible(true);
    ui->tableView->setColumnWidth(0,350);

    tableModel2 = new QStandardItemModel();
    tableModel2->setColumnCount(2);
    tableModel2->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("选中过的"));
    tableModel2->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("时间"));
    ui->tableView_2->setModel(tableModel2);
    //表头信息显示居左
    ui->tableView_2->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
    ui->tableView_2->verticalHeader()->setVisible(true);
    ui->tableView_2->setColumnWidth(0,250);
    ui->tableView_2->setColumnWidth(1,130);

    this->ReadSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::WriteSettings()
{
    settings->clear();
    settings->beginGroup("ToSelect");
    for(int i=0;i<toSelectList.size();i++)
        settings->setValue("Item"+QString().setNum(i),toSelectList.at(i));
    settings->endGroup();

    settings->beginGroup("Select");
    for(int i=0;i<pastRecord.size();i++)
        settings->setValue("Item"+QString().setNum(i),pastRecord.at(i));
    settings->endGroup();
    settings->beginGroup("Config");
    settings->setValue("Record History",ui->spinBox->value());
    settings->endGroup();
}

void MainWindow::ReadSettings()
{
    QString tmp;
    settings->beginGroup("ToSelect");
    for(int i=0;;i++)
    {
        tmp=settings->value("Item"+QString().setNum(i)).toString();
        if(tmp!="")
        {
            toSelectList.append(tmp);
            tableModel->setItem(tableModel->rowCount(),new QStandardItem(tmp));
        }
        else break;
    }

    settings->endGroup();

    settings->beginGroup("Select");
    for(int i=0;;i++)
    {
        tmp=settings->value("Item"+QString().setNum(i)).toString();
        if(tmp!="")
        {
            pastRecord.append(tmp);
            tableModel2->setItem(tableModel2->rowCount(),0,new QStandardItem(tmp.mid(0,tmp.indexOf(";"))));
            tableModel2->setItem(tableModel2->rowCount()-1,1,new QStandardItem(tmp.mid(tmp.indexOf(";")+1)));
        }
        else break;
    }
    settings->endGroup();
    settings->beginGroup("Config");
    ui->spinBox->setValue(settings->value("Record History").toInt());
    settings->endGroup();
}
void MainWindow::UpdateList()
{
    toSelectList.clear();
    QString tmp;
    for(int i=0;i<tableModel->rowCount();i++)
    {
        tmp=tableModel->item(i)->text();
        if(tmp!="") toSelectList.append(tmp);
    }
    this->WriteSettings();
}

void MainWindow::on_pushButton_generate_clicked()
{
    this->UpdateList();
    if(pastRecord.size()>=toSelectList.size())
    {
        QMessageBox::warning(this, tr("Error!"),tr("No choice can be generated!"),QMessageBox::Ok);
        return;
    }
    int n;
    n = qrand()%toSelectList.size();
    for(int i=0;i<pastRecord.size();)
    {
        if(pastRecord.at(i).startsWith(toSelectList.at(n)))
        {
            n = qrand()%toSelectList.size();
            i=0;
            continue;
        }
        i++;
    }
    int ret = QMessageBox::question(this, tr("We have got a choice!"),"Our choice is "+toSelectList.at(n)+"\nAccept it?",
                                   QMessageBox::Accepted,QMessageBox::Cancel);
    if(ret==QMessageBox::Accepted)
    {
        QString tmp=QDate::currentDate().toString(Qt::ISODate)+" "+QTime::currentTime().toString();
        pastRecord.append(toSelectList.at(n)+";"+tmp);
        tableModel2->setItem(tableModel2->rowCount(),0,new QStandardItem(toSelectList.at(n)));
        tableModel2->setItem(tableModel2->rowCount()-1,1,new QStandardItem(tmp));
        while(pastRecord.size()>ui->spinBox->value())
        {
            pastRecord.removeFirst();
            tableModel2->removeRow(0);
        }
        this->WriteSettings();
    }
}



void MainWindow::on_pushButton_Insert_clicked()
{
    tableModel->setItem(tableModel->rowCount(),new QStandardItem(""));
}


