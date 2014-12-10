#ifndef DEVICE_SETTINGS_H
#define DEVICE_SETTINGS_H

#include <QDialog>

#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>


namespace Ui {
  class DeviceSettings;
}

struct DeviceSettingsPacket
{
  QSerialPort::BaudRate    baudRate;
  QSerialPort::DataBits    dataBits;
  QSerialPort::Parity      parity;
  QSerialPort::StopBits    stopBits;
  QSerialPort::FlowControl flowControl;
};

class DeviceSettings : public QDialog
{
    Q_OBJECT

  public:
    explicit DeviceSettings(QWidget *parent = 0);
    ~DeviceSettings();

  private:
    Ui::DeviceSettings *ui;
    DeviceSettingsPacket  deviceSettings;
    DeviceSettingsPacket* currentSettings;

  signals:
    void saveSettings(DeviceSettingsPacket settings);
  private slots:
    void on_buttonBox_accepted();
    void on_baudRate_comboBox_activated(const QString &arg1);
    void on_dataBits_comboBox2_activated(const QString &arg1);
    void on_parity_comboBox4_activated(int index);
    void on_flowControl_comboBox5_activated(int index);
    void on_stopBits_comboBox3_activated(int index);

  public slots:
    void initSettings(const DeviceSettingsPacket& settings);
};

#endif // DEVICE_SETTINGS_H
