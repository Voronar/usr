/*--------------------------------(C)2015 Kirill A. Khalitov--------------------------------------*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTimer>
#include <QDataStream>
#include <QFile>


#include <QDebug>

#include "device_settings.h"

const unsigned short MAX_PACKET_CLIENT = 1024;

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

  private:
    Ui::MainWindow* ui;
    DeviceSettings* dsWindow;
    //------------------------------------FIELDS------------------------------------//
    QSerialPort*         device;
    QString              currentDeviceName;
    DeviceSettingsPacket deviceSettings;

    QTimer* savingTimer;
    QTimer* serialTimer;
    QFile*  logFile;
    QString fileName;
    quint32 saveDuration;
    quint16 durationCoefficient;
    bool    savingState;
    bool    dataToSave;
    //------------------------------------&FIELDS--------------------------------//

    //------------------------------------PRIVATE_METHODS------------------------------------//
    void detectDevices();
    void saveToFile(QString text);
    //------------------------------------&PRIVATE_METHODS------------------------------------//

  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    const DeviceSettingsPacket& getCurrentSettings();

    //------------------------------------PUBLIC_METHODS------------------------------------//

    //------------------------------------&PUBLIC_METHODS------------------------------------//

  private slots:
    void on_openButton_clicked();
    void on_devList_activated(const QString &arg1);
    void on_closeButton_clicked();
    void on_refreshButton_clicked();
    void dataAvailability();
    void dataReadiness();
    void on_clearConsole_Button_clicked();
    void on_settings_pushButton_clicked();
    void on_savingStart_Button_clicked();
    void on_saveDuration_comboBox_activated(int index);
    void on_savingStop_Button_clicked();

    void deviceSettings_changed(DeviceSettingsPacket settings);
    void readyToSave();



signals:
    void deviceSettings_init(const DeviceSettingsPacket& settings);

};
#endif // MAINWINDOW_H
