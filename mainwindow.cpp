#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Set window title.
    setWindowTitle(tr("omx-control"));
    ui->lineEdit_server->setText("raspberrypi");
    ui->lineEdit_port->setText("22");
    ui->lineEdit_user->setText("pi");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_connect_clicked()
{
    QString localcommand;
    QStringList sshcommand;
    // Generate the play command.
    gencommand_play(localcommand);
    // Generate the ssh command.
    gencommand_ssh(localcommand, sshcommand);

    // Start ssh with parameters.
    sshprocess = new QProcess();
    sshprocess->start("ssh", sshcommand);
}

void MainWindow::on_pushButton_stop_clicked()
{
    sshprocess->write("q");
    sshprocess->closeWriteChannel();
}

void MainWindow::gencommand_play(QString &commandline)
{
    // Process numbers for timestamp.
    QString hours = ui->spinBox_hrs->text();
    hours = hours.rightJustified(2, '0');
    QString mins = ui->spinBox_mins->text();
    mins = mins.rightJustified(2, '0');
    QString secs = ui->spinBox_secs->text();
    secs = secs.rightJustified(2, '0');

    // Process url
    QString url = ui->plainTextEdit_url->toPlainText();

    // Create command line to send through ssh.
    commandline = "yt "+url+" "+hours+":"+mins+":"+secs;
    qDebug() << commandline;
}

void MainWindow::gencommand_ssh(QString &command, QStringList &sshcommand)
{
    // Get variables from input.
    QString server = ui->lineEdit_server->text();
    QString port = ui->lineEdit_port->text();
    QString username = ui->lineEdit_user->text();

    sshcommand = QStringList()<<"-p"<<port<<"-t"<<"-l"<<username<<server<<command;
    qDebug() << sshcommand;
}
