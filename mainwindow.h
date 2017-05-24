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
    void on_pushButton_kill_clicked();
    void on_pushButton_pause_clicked();
    void on_pushButton_n30s_pressed();
    void on_pushButton_p30s_pressed();
    void on_pushButton_n10m_pressed();
    void on_pushButton_p10m_pressed();

private:
    Ui::MainWindow *ui;
    QProcess* sshprocess;
    void gencommand_play(QString &commandline);
    void gencommand_ssh(QString &command, QStringList &sshcommand);
};

#endif // MAINWINDOW_H
