#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QShortcut>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Read Stored Settings.
    readSettings();
    // Set window title.
    setWindowTitle(tr("omx-control"));
}

MainWindow::~MainWindow()
{
    // Write modified settings.
    writeSettings();
    delete ui;
}

void MainWindow::gencommand_localplay(QString &commandline)
{
    // Get time
    QString time = genstring_time();
    // Process url
    QString url = genstring_baseurl();

    // Run the command.
    QProcess ytdl_process;
    ytdl_process.start("youtube-dl", QStringList() <<"-g"<<"-f"<<"best"<<url);
    ytdl_process.waitForFinished(30000);
    QString ytdl_omxurl = ytdl_process.readAllStandardOutput();
    ytdl_omxurl = ytdl_omxurl.trimmed();
    QString ytdl_error = ytdl_process.readAllStandardError();
    qDebug() << ytdl_error;

    // Create command line to send through ssh.
    commandline = "omxplayer -o both \'"+ytdl_omxurl+"\' -l "+time;
}

QString MainWindow::genstring_time()
{
    // Process numbers for timestamp.
    QString hours = ui->spinBox_hrs->text();
    hours = hours.rightJustified(2, '0');
    QString mins = ui->spinBox_mins->text();
    mins = mins.rightJustified(2, '0');
    QString secs = ui->spinBox_secs->text();
    secs = secs.rightJustified(2, '0');

    return hours+":"+mins+":"+secs;
}

QString MainWindow::genstring_baseurl()
{
    // Process url
    QString url;
    url = ui->plainTextEdit_url->toPlainText();
    // Remove everything after the & symbol.
    QStringList url_list = url.split('&');
    url = url_list[0];
    return url;
}

void MainWindow::gencommand_ssh(QString &command, QStringList &sshcommand)
{
    // Get variables from input.
    QString server = ui->lineEdit_server->text();
    QString port = ui->lineEdit_port->text();
    QString username = ui->lineEdit_user->text();
    // Add variables to ssh command.
    sshcommand = QStringList()<<"-p"<<port<<"-t"<<"-l"<<username<<server<<command;
}

void MainWindow::sendcommand_ssh(QStringList &command)
{
    // Start ssh with parameters.
    // https://stackoverflow.com/questions/20743793/how-to-detect-when-ssh-connection-over-a-qprocess-has-finished
    // Output command to debug log.
    ui->textEdit_debugout->setText(command.join(" "));
    sshprocess = new QProcess();
    // Merge the Standard Output and Standard Error process channels.
    sshprocess->setProcessChannelMode(QProcess::MergedChannels);
    // Print the standard output when a message is ready.
    connect(sshprocess, SIGNAL(readyReadStandardOutput()), this, SLOT(update_stdout()));
    sshprocess->start("ssh", command);
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
    sendcommand_ssh(sshcmd);
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

void MainWindow::on_pushButton_localconnect_clicked()
{
    QString localcommand;
    QStringList sshcommand;
    // Generate the play command.
    gencommand_localplay(localcommand);
    // Generate the ssh command.
    gencommand_ssh(localcommand, sshcommand);
    // Send the ssh command.
    sendcommand_ssh(sshcommand);
}

void MainWindow::writeSettings()
{
    // Construct settings, ini only.
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, ORGNAME, APPNAME);
    settings.beginGroup("MainWindow");
    // Window size and position.
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    // Server, port, user, and url.
    settings.setValue("server", ui->lineEdit_server->text());
    settings.setValue("port", ui->lineEdit_port->text());
    settings.setValue("user", ui->lineEdit_user->text());
    settings.setValue("url", ui->plainTextEdit_url->toPlainText());
    settings.endGroup();
}

void MainWindow::readSettings()
{
    // Construct settings, ini only.
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, ORGNAME, APPNAME);
    settings.beginGroup("MainWindow");
    // Window size and position.
    QSize size = settings.value("size").toSize();
    // Resize only if non-default settings detected.
    if(size != QSize(-1, -1))
        resize(size);
    QPoint point = settings.value("pos").toPoint();
    // Move only if non-default settings detected.
    if(point != QPoint(0,0))
        move(point);
    // Server, port, user, and url.
    ui->lineEdit_server->setText(settings.value("server").toString());
    ui->lineEdit_port->setText(settings.value("port", "22").toString());
    ui->lineEdit_user->setText(settings.value("user").toString());
    ui->plainTextEdit_url->setPlainText(settings.value("url").toString());
    settings.endGroup();
}

void MainWindow::update_stdout()
{
    // Output the standard output to the stdout box.
    QString stdout = sshprocess->readAllStandardOutput();
    ui->textEdit_stdout->setText(stdout);
}
