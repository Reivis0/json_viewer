#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  QWidget *central = centralWidget();
  {
    if(!central)
    {
      central = new QWidget(this);
      setCentralWidget(central);
    }
  }


  QVBoxLayout *mainLayout = new QVBoxLayout(central);
  QHBoxLayout *buttonsLayout = new QHBoxLayout();


  buttonsLayout->addWidget(ui->openButton);
  buttonsLayout->addStretch();
  buttonsLayout->addWidget(ui->showButton);
  buttonsLayout->addWidget(ui->updateButton);
 
  buttonsLayout->addStretch(); 


  QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
  splitter->addWidget(ui->jsonTextEdit);
  splitter->addWidget(ui->jsonTreeView);
  
  splitter->setStretchFactor(0, 1);
  splitter->setStretchFactor(1, 1);


  mainLayout->addLayout(buttonsLayout);
  mainLayout->addWidget(splitter);



  ui->openButton->setIcon(QIcon(IMAGE_OPEN_FILE_PATH));
  ui->openButton->setIconSize(QSize(16, 16));
  ui->showButton->setIcon(QIcon(IMAGE_EXPAND_FILE_PATH));
  ui->showButton->setIconSize(QSize(16, 16));
  ui->showButton->setText("Развернуть все");


  ui->updateButton->setIcon(QIcon(IMAGE_UPDATE_FILE_PATH));
  ui->updateButton->setIconSize(QSize(16, 16));


  m_highlighter.setDocument(ui->jsonTextEdit->document());
}


MainWindow::~MainWindow()
{
  delete ui;
}


void MainWindow::on_openButton_clicked()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Выберить JSON-файл"), "", tr("JSON (*.json)"));
    if (!fileName.isEmpty())
    {
      QFile file(fileName);
      if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) 
      {
        QMessageBox::warning(this, tr("Ошибка"), tr("Не возможно открыть файл: ") + fileName);
        return;
      }
      QTextStream in(&file);
      QString jsonString = in.readAll();
      file.close();
      QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
      if (jsonDoc.isNull())
      {
        QMessageBox::warning(this, tr("Ошибка"), tr("Некорректный JSON формат"));
        return;
      }
      ui->jsonTextEdit->setPlainText(jsonString);
      if (m_model.rootIndex().isValid()) 
      {
        m_model.clear();
      }

      // ИСПРАВЛЕНИЕ: Используем loadJson для сохранения порядка
      m_model.loadJson(jsonString.toUtf8());
      
      ui->jsonTreeView->setModel(&m_model);
      ui->showButton->setIcon(QIcon(IMAGE_EXPAND_FILE_PATH));
      ui->showButton->setText("Развернуть все");
    }
    else
    {
        qDebug()<<"Предупреждение: файл пуст";
    }
}


void MainWindow::on_updateButton_clicked()
{
  QString jsonString = ui->jsonTextEdit->toPlainText();
  QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
  if (jsonDoc.isNull())
  {
    QMessageBox::warning(this, tr("Ошибка"), tr("Некорректный JSON формат"));
    return;
  }
  if (m_model.rootIndex().isValid())
  {
    m_model.clear();
  }

  // ИСПРАВЛЕНИЕ: Используем loadJson для сохранения порядка
  m_model.loadJson(jsonString.toUtf8());


  ui->jsonTreeView->setModel(&m_model);
  ui->showButton->setIcon(QIcon(IMAGE_EXPAND_FILE_PATH));
  ui->showButton->setText("Развернуть все");
}


bool MainWindow::isTreeExpanded(const QModelIndex &index)
{
  if (!ui->jsonTreeView->model()->hasChildren(index))
  {
    return true;
  }
  else
  {
    if (!ui->jsonTreeView->isExpanded(index))
    {
      return false;
    }
  }
  int count = ui->jsonTreeView->model()->rowCount(index);
  for (int i = 0; i < count; ++i)
  {
    QModelIndex childIndex = ui->jsonTreeView->model()->index(i, 0, index);
    if (!isTreeExpanded(childIndex))
    {
      return false;
    }
  }
  return true;
}


bool MainWindow::isTreeCollapsed(const QModelIndex &index)
{
  if (!ui->jsonTreeView->isExpanded(index))
  {
    return true;
  }
  int count = ui->jsonTreeView->model()->rowCount(index);
  for (int i = 0; i < count; ++i)
  {
    QModelIndex childIndex = ui->jsonTreeView->model()->index(i, 0, index);
    if (isTreeCollapsed(childIndex))
    {
      return true;
    }
  }
  return false;
}


void MainWindow::expandAll(const QModelIndex &index)
{
  ui->jsonTreeView->expand(index);
  int count = ui->jsonTreeView->model()->rowCount(index);
  for (int i = 0; i < count; ++i)
  {
    QModelIndex childIndex = ui->jsonTreeView->model()->index(i, 0, index);
    expandAll(childIndex);
  }
}


void MainWindow::collapseAll(const QModelIndex &index)
{
  ui->jsonTreeView->collapse(index);
  int count = ui->jsonTreeView->model()->rowCount(index);
  for (int i = 0; i < count; ++i)
  {
    QModelIndex childIndex = ui->jsonTreeView->model()->index(i, 0, index);
    collapseAll(childIndex);
  }
}


void MainWindow::on_showButton_clicked()
{
  if (m_model.rowCount() != 0)
  {
    if (ui->showButton->text() == "Развернуть все")
    {
      for (int i = 0; i <= ui->jsonTreeView->model()->columnCount(); i++)
      {
        expandAll(ui->jsonTreeView->model()->index(0, i));
      }
      ui->showButton->setIcon(QIcon(IMAGE_COLLAPSE_FILE_PATH));
      ui->showButton->setText("Свернуть все");
    }
    else
    {
      for (int i = 0; i <= ui->jsonTreeView->model()->columnCount(); i++)
      {
        collapseAll(ui->jsonTreeView->model()->index(0, i));
      }
      ui->showButton->setIcon(QIcon(IMAGE_EXPAND_FILE_PATH));
      ui->showButton->setText("Развернуть все");
    }
  }
  else
  {
    QMessageBox::warning(this, tr("Ошибка"), tr("Дерево не содержит данных"));
    return;
  }
}


void MainWindow::on_jsonTreeView_collapsed(const QModelIndex &index)
{
  bool check = true;
  for (int i = 0; i < ui->jsonTreeView->model()->columnCount(); i++)
  {
    check = isTreeCollapsed(ui->jsonTreeView->model()->index(0, i));
    if (!check)
    {
      return;
    }
  }
   ui->showButton->setIcon(QIcon(IMAGE_EXPAND_FILE_PATH));
   ui->showButton->setText("Развернуть все");
}


void MainWindow::on_jsonTreeView_expanded(const QModelIndex &index)
{
  bool check = true;
  for (int i = 0; i < ui->jsonTreeView->model()->columnCount(); i++)
  {
    check = isTreeExpanded(ui->jsonTreeView->model()->index(0, i));
    if (!check)
    {
      return;
    }
  }
  ui->showButton->setIcon(QIcon(IMAGE_COLLAPSE_FILE_PATH));
  ui->showButton->setText("Свернуть все");
}
