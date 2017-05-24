#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QDebug>
#include <QShortcut>

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
    // Remove everything after the & symbol.
    QStringList url_list = url.split('&');
    qDebug() << url_list;
    url = url_list[0];

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

void MainWindow::on_pushButton_connect_clicked()
{
    QString localcommand;
    QStringList sshcommand;
    // Generate the play command.
    gencommand_play(localcommand);
    // Generate the ssh command.
    gencommand_ssh(localcommand, sshcommand);

    // Start ssh with parameters.
    // https://stackoverflow.com/questions/20743793/how-to-detect-when-ssh-connection-over-a-qprocess-has-finished
    sshprocess = new QProcess();
    sshprocess->start("ssh", sshcommand);
}

void MainWindow::on_pushButton_stop_clicked()
{
    sshprocess->write("q");
    sshprocess->closeWriteChannel();
}

void MainWindow::on_pushButton_kill_clicked()
{
    QString kill_command = "killall omxplayer.bin";
    QStringList sshcmd;

    // Generate SSH arguments.
    gencommand_ssh(kill_command, sshcmd);

    // Start ssh with parameters.
    sshprocess = new QProcess();
    sshprocess->start("ssh", sshcmd);
}

void MainWindow::on_pushButton_pause_clicked()
{
    sshprocess->write(" ");
}

void MainWindow::on_pushButton_n30s_pressed()
{
    sshprocess->write("^[[D");
}

void MainWindow::on_pushButton_p30s_pressed()
{
    sshprocess->write("^[[C");
}

void MainWindow::on_pushButton_n10m_pressed()
{
    sshprocess->write("^[[B");
}

void MainWindow::on_pushButton_p10m_pressed()
{
    sshprocess->write("^[[A");
}

void MainWindow::on_lineEdit_dragdropurl_textEdited()
{
    ui->plainTextEdit_url->setPlainText(ui->lineEdit_dragdropurl->text());
    ui->lineEdit_dragdropurl->clear();
}
