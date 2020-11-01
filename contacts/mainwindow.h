#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>


#include <QLabel>
#include <QLineEdit>
#include <QTableView>
#include <QTextEdit>
#include <QPushButton>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //static QByteArray& decode(const QString &input);

private slots:
    int read(QRegExp, int s); // считывает тел. и имя
    void read_p(QRegExp rt, int s); // считывает код картинки

    void on_tableView_clicked(const QModelIndex &index);

    void on_isk_cursorPositionChanged(int arg1, int arg2);

    void on_pushButton_clicked();

    void import_kont();

private:
    Ui::MainWindow *ui;
};

class QuotedPrintable : public QObject
{
    Q_OBJECT
public:

    static QString& encode(const QByteArray &input);
    static QByteArray& decode(const QString &input);

private:
    QuotedPrintable();
};

#endif // MAINWINDOW_H
