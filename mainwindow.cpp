#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mqttClient = new QMqttClient(this);
    mqttClient->setHostname("192.168.1.100");
    mqttClient->setPort(1883);
    mqttTopic.setName("SteamDeckInput");

    connect(mqttClient, &QMqttClient::stateChanged, this, &MainWindow::updateLogStateChange);
    connect(mqttClient, &QMqttClient::disconnected, this, &MainWindow::brokerDisconnected);
    connect(mqttClient, &QMqttClient::messageReceived, this, [this](const QByteArray &message, const QMqttTopicName &topic) {
        const QString content = QDateTime::currentDateTime().toString()
                                + " Received Topic: "
                                + topic.name()
                                + " Message: "
                                + message
                                + u'\n';
        ui->debugText->insertPlainText(content);
        ui->debugTest->show();
    });

    // connect(mqttClient, &QMqttClient::pingResponseReceived, this, [this]() {
    //     const QString content = QDateTime::currentDateTime().toString()
    //                             + "PingResponse\n";
    //     Q_UNUSED(this)
    //     // ui->debugTest->insertPlainText(content);
    // });
    QMqttTopicFilter topic;
    mqttClient->connectToHost();
    topic.setFilter("*");
    auto subs = mqttClient->subscribe(topic, 0);
}

MainWindow::~MainWindow()
{
    delete ui;
    mqttClient->disconnectFromHost();
}

void MainWindow::updateLogStateChange()
{
    const QString content = QDateTime::currentDateTime().toString()
                            + ": State Change"
                            + QString::number(mqttClient->state())
                            + u'\n';Q_UNUSED(content);
    // ui->line2_row2_button->setText(content);
}

void MainWindow::brokerDisconnected()
{

}
