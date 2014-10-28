#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QTime>
#include <QMap>
#include <QModelIndex>
#include <QTableView>
#include <QStandardItemModel>
#include <QDebug>
#include <QList>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void ReadSettings();
    void WriteSettings();

private slots:
    void UpdateList();

    void on_pushButton_generate_clicked();

    void on_pushButton_Insert_clicked();


private:
    Ui::MainWindow *ui;
    QSettings *settings;
    QList<QString> pastRecord;
    QStandardItemModel *tableModel,*tableModel2;
    QList<QString> toSelectList;
};

#endif // MAINWINDOW_H
