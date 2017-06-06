#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QSettings>
#include <QTimer>

#define APPNAME "omx-control"
#define ORGNAME "omx-control"

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
    void on_pushButton_stop_clicked();
    void on_pushButton_kill_clicked();
    void on_pushButton_pause_clicked();
    void on_pushButton_n30s_pressed();
    void on_pushButton_p30s_pressed();
    void on_pushButton_n10m_pressed();
    void on_pushButton_p10m_pressed();
    void on_lineEdit_dragdropurl_textEdited();
    void on_pushButton_localconnect_clicked();
    void update_stdout();

private:
    Ui::MainWindow *ui;
    QProcess* sshprocess = new QProcess();
    void gencommand_localplay(QString &commandline);
    void gencommand_ssh(QString &command, QStringList &sshcommand);
    void sendcommand_ssh(QStringList &command);
    QString genstring_time();
    QString genstring_baseurl();
    void writeSettings();
    void readSettings();
};

#endif // MAINWINDOW_H
