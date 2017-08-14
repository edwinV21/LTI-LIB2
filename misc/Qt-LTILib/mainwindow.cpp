#include "ltiQt.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <ltiVector.h>
#include <ltiMatrix.h>
#include <ltiImage.h>
#include <ltiUniformContinuousDistribution.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }


}

void MainWindow::on_actionVector_triggered(bool ) {
    lti::matrix<float> m(3,3,1);
    std::cout << m << std::endl;
}

void MainWindow::on_actionMatrix_triggered(bool ) {
    lti::vector<float> v(5,2);
    std::cout << v << std::endl;
}

void MainWindow::on_actionImage_triggered(bool ) {
  // a simple random number generator
  static lti::uniformContinuousDistribution rnd(0.0,255.0);

  // create a 256x256 large image filled in with a random color
  lti::image v(256,256,lti::rgbaPixel(rnd.frand(),rnd.frand(),rnd.frand()));

  // use the provided method to convert the image to a QPixmap
  QPixmap qtPx = convertImageToQPixmap(v);

  ui->imgLabel->setPixmap(qtPx);
  ui->imgLabel->show();
}
