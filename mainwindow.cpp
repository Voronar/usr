/*--------------------------------(C)2015 Kirill A. Khalitov--------------------------------------*/
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  //Serial port settings
  device = new QSerialPort(this);
  currentDeviceName = "";
  deviceSettings.baudRate    = QSerialPort::Baud115200;
  deviceSettings.dataBits    = QSerialPort::Data8;
  deviceSettings.stopBits    = QSerialPort::OneStop;
  deviceSettings.parity      = QSerialPort::NoParity;
  deviceSettings.flowControl = QSerialPort::NoFlowControl;

  if(!QDir("log").exists())
  {
    QDir().mkdir("log");
  }

  qDebug() << "start";
  logFile = new QFile(QDir::current().path() + "/log/" + QDateTime::currentDateTime().toString("dd-m-yyyy hh-mm-ss") + ".txt", this);

  durationCoefficient = 1000;
  QIntValidator* intValidator = new QIntValidator(0, 2147483647, this);
  ui->durationEdit->setValidator(intValidator);

  serialTimer = new QTimer(this);
  serialTimer->setInterval(500);

  savingTimer = new QTimer(this);
  savingTimer->setInterval(ui->durationEdit->text().toInt() * durationCoefficient);
  savingState = false;
  dataToSave  = false;

  detectDevices();

  //serial port
  connect(device, &QSerialPort::readyRead,  this, &MainWindow::dataAvailability);
  connect(serialTimer, &QTimer::timeout,  this, &MainWindow::dataReadiness);
  connect(savingTimer, &QTimer::timeout,  this, &MainWindow::readyToSave);
}

MainWindow::~MainWindow()
{
  if(device->isOpen())
  {
    device->clear();
    device->close();
    qDebug() << "Closing [OK]";
  }
  delete ui;
}

void MainWindow::dataAvailability()
{
  qDebug() << device->bytesAvailable();
  if(device->bytesAvailable() < MAX_PACKET_CLIENT)
  {
    if(!serialTimer->isActive())
      serialTimer->start();
  }
  else
  {
    dataReadiness();
  }
}

void MainWindow::dataReadiness()
{
  serialTimer->stop();

  QString     stringData;

  stringData = device->readAll();


  if(stringData != "")
  {
    ui->console->appendPlainText(stringData);

    dataToSave = true;

    if(savingState)
    {
      saveToFile(stringData);
    }
  }
}

const DeviceSettingsPacket& MainWindow::getCurrentSettings()
{
  return deviceSettings;
}

void MainWindow::on_openButton_clicked()
{
  if(!device->isOpen())
  {
    if(currentDeviceName == "")
    {
      ui->console->appendPlainText("Set a device name");
    }
    else
    {
      device->setPortName(currentDeviceName);
      if(device->open(QIODevice::ReadWrite))
      {
        ui->console->appendPlainText("Opening [OK]");
        device->setBaudRate    (deviceSettings.baudRate);
        device->setDataBits    (deviceSettings.dataBits);
        device->setStopBits    (deviceSettings.stopBits);
        device->setParity      (deviceSettings.parity);
        device->setFlowControl (deviceSettings.flowControl);
      }
      else
      {
        ui->console->appendPlainText("Can not open the '" + currentDeviceName + "' [ERROR]");
      }
    }
  }
  else
  {
    ui->console->appendPlainText("Already openned");
  }
}


void MainWindow::on_closeButton_clicked()
{
  if(device->isOpen())
  {
    dataToSave = false;
    device->clear();
    device->close();
    ui->console->appendPlainText("Closing [OK]");
  }
  else
  {
    ui->console->appendPlainText("Device is already closed or is not opened yet");
  }
}


void MainWindow::on_devList_activated(const QString &arg1)
{
  currentDeviceName = arg1;
}


void MainWindow::saveToFile(QString text)
{
  if(ui->textFilename->text() != "")
  {
    //logFile->setFileName(QDir::current().path() + "/log/" + ui->textFilename->text());
    logFile->setFileName(ui->textFilename->text());
  }


  if(!logFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
  {
    ui->console->appendPlainText("Unable to open '" + logFile->fileName() + "'' file");
    return;
  }

  QTextStream  file(logFile);

  //file <<  QDateTime::currentDateTime().toString("dd-m-yyyy hh:mm:ss") + " ";
  file << text << "\n";

  logFile->close();
}

void MainWindow::detectDevices()
{
  QStringList devices;

  for (QSerialPortInfo dev : QSerialPortInfo::availablePorts())
  {
    devices += dev.portName();
  }
  ui->devList->addItems(devices);
}


void MainWindow::on_clearConsole_Button_clicked()
{
  ui->console->clear();
}

void MainWindow::on_settings_pushButton_clicked()
{
  dsWindow = new DeviceSettings (this);
  connect(dsWindow, &DeviceSettings::saveSettings, this, &MainWindow::deviceSettings_changed);
  connect(this, &MainWindow::deviceSettings_init, dsWindow, &DeviceSettings::initSettings);
  emit deviceSettings_init(deviceSettings);

  dsWindow->setModal(true);
  dsWindow->show();


}

void MainWindow::deviceSettings_changed(DeviceSettingsPacket settings)
{
  deviceSettings = settings;
}

void MainWindow::readyToSave()
{
  ui->console->appendPlainText("Saving has stoped");
  savingTimer->stop();
  savingState = false;
}

void MainWindow::on_savingStart_Button_clicked()
{
  if(dataToSave)
  {
    if(!savingState)
    {
      ui->console->appendPlainText("Saving is running...");
      savingTimer->start(ui->durationEdit->text().toInt() * durationCoefficient);
      savingState = true;
    }
    else
    {
      ui->console->appendPlainText("Saving is already running");
    }
  }
  else
  {
    ui->console->appendPlainText("Nothing to save");
  }
}

void MainWindow::on_savingStop_Button_clicked()
{
  if(savingState)
  {
    ui->console->appendPlainText("Saving is terminating...");
    savingTimer->stop();
    savingState = false;
  }
  else
  {
    ui->console->appendPlainText("Saving is already terminated or not running yet");
  }
}

void MainWindow::on_saveDuration_comboBox_activated(int index)
{
  switch (index) {
    case 0:
      durationCoefficient = 1000;
    break;
    case 1:
      durationCoefficient = 1;
    break;
  }
}

void MainWindow::on_refreshButton_clicked()
{
  ui->devList->clear();
  currentDeviceName = "";
  detectDevices();
}
