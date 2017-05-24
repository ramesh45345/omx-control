#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_connect_clicked();
    void on_pushButton_stop_clicked();

private:
    Ui::MainWindow *ui;
    QProcess* sshprocess;
    void gencommand_play(QString &commandline);
    void gencommand_ssh(QString &command, QStringList &sshcommand);
};

#endif // MAINWINDOW_H
