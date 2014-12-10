#include "device_settings.h"
#include "ui_device_settings.h"

DeviceSettings::DeviceSettings(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DeviceSettings)
{
  ui->setupUi(this);

  this->setFixedSize(400, 300);

  deviceSettings.baudRate    = QSerialPort::Baud115200;
  deviceSettings.dataBits    = QSerialPort::Data8;
  deviceSettings.stopBits    = QSerialPort::OneStop;
  deviceSettings.parity      = QSerialPort::NoParity;
  deviceSettings.flowControl = QSerialPort::NoFlowControl;


  ui->parity_comboBox4->addItem("No",    QVariant(0));
  ui->parity_comboBox4->addItem("Even",  QVariant(2));
  ui->parity_comboBox4->addItem("Odd",   QVariant(3));
  ui->parity_comboBox4->addItem("Space", QVariant(4));
  ui->parity_comboBox4->addItem("Mark",  QVariant(5));


}

DeviceSettings::~DeviceSettings()
{
  delete ui;
}

void DeviceSettings::on_buttonBox_accepted()
{
  /*
  deviceSettings.baudRate    = (QSerialPort::BaudRate)ui->baudRate_comboBox->currentText().toInt();
  deviceSettings.dataBits    = (QSerialPort::DataBits)ui->dataBits_comboBox2->currentText().toInt();
  deviceSettings.stopBits    = (QSerialPort::StopBits)ui->stopBits_comboBox3->currentText().toInt();
  deviceSettings.parity      = (QSerialPort::Parity)ui->parity_comboBox4->currentText().toInt();
  deviceSettings.flowControl = (QSerialPort::FlowControl)ui->flowControl_comboBox5->currentIndex();
  */


  emit saveSettings(deviceSettings);
}

void DeviceSettings::on_baudRate_comboBox_activated(const QString &arg1)
{
  deviceSettings.baudRate = (QSerialPort::BaudRate)arg1.toInt();
}

void DeviceSettings::on_dataBits_comboBox2_activated(const QString &arg1)
{
  deviceSettings.dataBits = (QSerialPort::DataBits)arg1.toInt();
}

void DeviceSettings::on_stopBits_comboBox3_activated(int index)
{
  deviceSettings.stopBits = (QSerialPort::StopBits)ui->stopBits_comboBox3->itemData(index).toInt();
}

void DeviceSettings::on_parity_comboBox4_activated(int index)
{
  switch (index) {
    case QSerialPort::NoParity:
      deviceSettings.parity = (QSerialPort::Parity)index;
    break;
    default:
      deviceSettings.parity = (QSerialPort::Parity)(index + 1);
    break;
  }

}

void DeviceSettings::on_flowControl_comboBox5_activated(int index)
{
  deviceSettings.flowControl = (QSerialPort::FlowControl)index;
}

void DeviceSettings::initSettings(const DeviceSettingsPacket& settings)
{
  ui->baudRate_comboBox->setCurrentText(QString::number(settings.baudRate));
  ui->dataBits_comboBox2->setCurrentText(QString::number(settings.dataBits));
  ui->stopBits_comboBox3->setCurrentText(QString::number(settings.stopBits));

  switch(settings.parity)
  {
    case QSerialPort::NoParity:
      ui->parity_comboBox4->setCurrentIndex(settings.parity);
    break;

    default:
      ui->parity_comboBox4->setCurrentIndex(settings.parity - 1);
    break;
  }

  ui->flowControl_comboBox5->setCurrentIndex(settings.flowControl);
}
