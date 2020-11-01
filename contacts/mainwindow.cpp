#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCoreApplication>
#include <QIODevice>
#include <QFile>
#include <QTextStream>
#include <QDebug>


QStandardItem *item;
QStandardItemModel *model;
QString arr[50][3],ARR[50][3];
int f;
QString address("00001.vcf");


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    import_kont();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    int strind=ui->tableView->currentIndex().row();
    QByteArray b64img = QByteArray::fromBase64(ARR[strind][2].toLatin1());
    QImage image = QImage::fromData(b64img, "jpg");
    ui->label->setPixmap(QPixmap::fromImage(image));
}

QByteArray& QuotedPrintable::decode(const QString &input)
{
    //                    0  1  2  3  4  5  6  7  8  9  :  ;  <  =  >  ?  @  A   B   C   D   E   F
    const int hexVal[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15};

    QByteArray *output = new QByteArray();

    for (int i = 0; i < input.length(); ++i)
    {
        if (input.at(i).toLatin1() == '=')
        {
            output->append((hexVal[input.at(++i).toLatin1() - '0'] << 4) + hexVal[input.at(++i).toLatin1() - '0']);
        }
        else
        {
            output->append(input.at(i).toLatin1());
        }
    }

    return *output;
}

void MainWindow::on_isk_cursorPositionChanged(int arg1, int arg2)
{
    QRegExp findind("\\w*\\"+ui->isk->text()+"\\w*");
    bool g = true;
    int p = 0;
    for(int i = 0; i < f; i++)
    {
        if ((findind.indexIn(arr[i][0]) != -1) && (ui->isk->text() != ""))
        {
            if (g)
            {
                model->clear();
                model->setHorizontalHeaderLabels(QStringList() << "Имя"<<"Номер"<<"Фото" );
                ui-> tableView->setModel(model);
                g = false;
            }
            item = new QStandardItem(QString(arr[i][0]));
            model->setItem(p, 0, item);

            item = new QStandardItem(QString(arr[i][1]));
            model->setItem(p, 1, item);

            item = new QStandardItem(QString(arr[i][2]));
            model->setItem(p, 2, item); ARR[p][2]=arr[i][2];
            p++;
        }
        if (ui->isk->text() == "")
        {
            if (g)
            {
                model->clear();
                model->setHorizontalHeaderLabels(QStringList() << "Имя"<<"Номер"<<"Фото" );
                ui-> tableView->setModel(model);
                g = false;
            }
            item = new QStandardItem(QString(arr[i][0]));
            model->setItem(i, 0, item);

            item = new QStandardItem(QString(arr[i][1]));
            model->setItem(i, 1, item);

            item = new QStandardItem(QString(arr[i][2]));
            model->setItem(i, 2, item); ARR[i][2]=arr[i][2];
        }
        if ((findind.indexIn(arr[i][0]) == -1) && (ui->isk->text() != ""))
        {
            if (g)
            {
                model->clear();
                model->setHorizontalHeaderLabels(QStringList() << "Имя"<<"Номер"<<"Фото" );
                ui-> tableView->setModel(model);
                g = false;
            }
        }
        update();
    }
    update();
}

void MainWindow::import_kont()
{
    QRegExp rt("FN:(.*)"); // имя
    QRegExp rt1("CELL:(.*)"); // тел.
    QRegExp rt2("JPEG:"); // картинка

        f = read  (rt1,1); // или read(rt или rt1), зависит от того, что считываете
            read  (rt,0);
            read_p(rt2,2);
    model = new QStandardItemModel(this);
    model->setColumnCount(3);
    model->setHorizontalHeaderLabels(QStringList() << "Имя"<<"Номер"<<"Фото" );
    ui-> tableView->setModel(model);

    for(int i = 0; i < f; i++)
    {
        arr[i][0] = QuotedPrintable::decode(arr[i][0]);
        item = new QStandardItem(QString(arr[i][0]));
        model->setItem(i, 0, item);

        item = new QStandardItem(QString(arr[i][1]));
        model->setItem(i, 1, item);

        item = new QStandardItem(QString(arr[i][2]));
        model->setItem(i, 2, item); ARR[i][2]=arr[i][2];
    }
}

int MainWindow::read(QRegExp rt, int s)
{
    QString str0 = "", str;
    QFile file(address);

    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);

    int i = 0;

    while (!in.atEnd())
    {
        in >> str;
        str.replace(" ", "");
        if (-1 != rt.indexIn(str))
        {
            str = rt.cap(1);
            if (str != str0)
            {
                arr[i][s] = str;

                str0 = str;
                i++;
            }
        }
    }
    file.close();
    return i;
}

void MainWindow::read_p(QRegExp rt, int s)
{
    QString str;
    QFile file(address);
    QRegExp rt1("END:VCARD");

    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);

    int i=0;
    while (!in.atEnd())
    {
        in >> str;

        if (-1 != rt.indexIn(str))
        {
            str.replace("PHOTO;ENCODING=B;TYPE=JPEG:", "");
            arr[i][s] = str;

            while (-1 == rt1.indexIn(str))
            {
                in >> str;
                arr[i][s] += str;

            }
            arr[i][s].replace("END:VCARD", "");
            i++;
        }
    }

    file.close();
}

void MainWindow::on_pushButton_clicked()
{
    model->clear();

    address = ui->lineEdit->text() + ".vcf";

    ui->isk->setText("");

    import_kont();
}
