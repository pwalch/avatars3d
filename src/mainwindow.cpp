/**
  * 3D Avatars
  * Pierre Walch
  */

#include <iostream>
#include <fstream>
#include <irrlicht.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "camerawindow.h"
#include "engine.h"
#include <QKeyEvent>
#include <QTime>
#include <QThread>

using namespace irr;
using namespace core;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // Qt window title
    ui->setupUi(this);
    this->setWindowTitle("Avatars controller");
    updateWidgets(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateWidgets(bool changeInit)
{
    CameraWindow& cam = CameraWindow::getInstance();

    this->setEnabled(false);

    // Update position widgets
    vector3df position = cam.getCameraPosition();
    ui->xPos->setValue(position.X);
    ui->yPos->setValue(position.Y);
    ui->zPos->setValue(position.Z);

    // Update rotation widgets
    vector3df rotation = cam.getCameraRotation();
    ui->xRot->setValue(rotation.X);
    ui->yRot->setValue(rotation.Y);
    ui->zRot->setValue(rotation.Z);

    int speed = cam.getSpeed();
    ui->speed->setValue(speed);

    // Update frame navigation widgets
    Engine& engine = Engine::getInstance();
    int frameNumber = engine.getFrameNumber();
    int currentTime = engine.getCurrentTime();
    int startTime = engine.getStartTime();
    int endTime = engine.getEndTime();

    ui->frameIndex->setMinimum(0);
    ui->frameIndex->setMaximum(frameNumber - 1);
    ui->fromVideo->setMinimum(0);
    ui->fromVideo->setMaximum(frameNumber - 2);
    ui->toVideo->setMinimum(1);
    ui->toVideo->setMaximum(frameNumber - 1);

    ui->fromVideo->setValue(startTime);
    ui->toVideo->setValue(endTime);
    ui->frameIndex->setValue(currentTime);

    // Set initial position if there is a reset
    if(changeInit) {
        initialPosition = position;
        initialRotation = rotation;
    }

    this->setEnabled(true);
}

void MainWindow::setCameraPosition(const vector3df& vector, bool updateScene)
{
    CameraWindow& cam = CameraWindow::getInstance();
    cam.setPosition(vector);
    if(updateScene)
        cam.updateScene();
}

void MainWindow::setCameraRotation(const vector3df& vector, bool updateScene)
{
    CameraWindow& cam = CameraWindow::getInstance();
    cam.setRotation(vector);
    if(updateScene)
        cam.updateScene();
}

void MainWindow::moveCamera(const vector3df& vector)
{
    CameraWindow& cam = CameraWindow::getInstance();
    cam.move(vector);
    // Camera move changes position and rotation, so we update them in the UI
    updateWidgets(false);
    cam.updateScene();
}

void MainWindow::rotateCamera(const vector3df& vector)
{
    CameraWindow& cam = CameraWindow::getInstance();
    cam.rotate(vector);
    updateWidgets(false);
    cam.updateScene();
}

void MainWindow::on_xPos_valueChanged(double arg1)
{
    vector3df currentPosition = CameraWindow::getInstance().getCameraPosition();
    currentPosition.X = (float)arg1;
    setCameraPosition(currentPosition, true);
}

void MainWindow::on_yPos_valueChanged(double arg1)
{
    vector3df currentPosition = CameraWindow::getInstance().getCameraPosition();
    currentPosition.Y = (float)arg1;
    setCameraPosition(currentPosition, true);
}

void MainWindow::on_zPos_valueChanged(double arg1)
{
    vector3df currentPosition = CameraWindow::getInstance().getCameraPosition();
    currentPosition.Z = (float)arg1;
    setCameraPosition(currentPosition, true);
}

void MainWindow::on_xRot_valueChanged(double arg1)
{
    vector3df currentRotation = CameraWindow::getInstance().getCameraRotation();
    currentRotation.X = (float)arg1;
    setCameraRotation(currentRotation, true);
}

void MainWindow::on_yRot_valueChanged(double arg1)
{
    vector3df currentRotation = CameraWindow::getInstance().getCameraRotation();
    currentRotation.Y = (float)arg1;
    setCameraRotation(currentRotation, true);
}

void MainWindow::on_zRot_valueChanged(double arg1)
{
    vector3df currentRotation = CameraWindow::getInstance().getCameraRotation();
    currentRotation.Z = (float)arg1;
    setCameraRotation(currentRotation, true);
}

void MainWindow::on_forwardPos_clicked()
{
    moveCamera(vector3df(this->ui->speed->value(), 0, 0));
}

void MainWindow::on_backwardsPos_clicked()
{
    moveCamera(vector3df(-this->ui->speed->value(), 0, 0));
}

void MainWindow::on_leftPos_clicked()
{
    moveCamera(vector3df(0, this->ui->speed->value(), 0));
}

void MainWindow::on_rightPos_clicked()
{
    moveCamera(vector3df(0, -this->ui->speed->value(), 0));
}

void MainWindow::on_upPos_clicked()
{
    moveCamera(vector3df(0, 0, this->ui->speed->value()));
}

void MainWindow::on_downPos_clicked()
{
    moveCamera(vector3df(0, 0, -this->ui->speed->value()));
}

void MainWindow::on_upRot_clicked()
{
    rotateCamera(vector3df(this->ui->speed->value(), 0, 0));
}

void MainWindow::on_downRot_clicked()
{
    rotateCamera(vector3df(-this->ui->speed->value(), 0, 0));
}

void MainWindow::on_leftRot_clicked()
{
    rotateCamera(vector3df(0, this->ui->speed->value(), 0));
}

void MainWindow::on_rightRot_clicked()
{
    rotateCamera(vector3df(0, -this->ui->speed->value(), 0));
}

void MainWindow::keyPressEvent(QKeyEvent * e)
{
    // Handling keyboard events for FPS camera
    switch(e->key()) {
        case Qt::Key_W:
            on_forwardPos_clicked();
            break;

        case Qt::Key_S:
            on_backwardsPos_clicked();
            break;

        case Qt::Key_A:
            on_leftPos_clicked();
            break;

        case Qt::Key_D:
            on_rightPos_clicked();
            break;

        case Qt::Key_E:
            on_upPos_clicked();
            break;

        case Qt::Key_C:
            on_downPos_clicked();
            break;

        case Qt::Key_I:
            on_upRot_clicked();
            break;

        case Qt::Key_K:
            on_downRot_clicked();
            break;

        case Qt::Key_J:
            on_leftRot_clicked();
            break;

        case Qt::Key_L:
            on_rightRot_clicked();
            break;

        default:
            break;
    }
}

void MainWindow::on_resetCamera_clicked()
{
    setCameraPosition(initialPosition, false);
    setCameraRotation(initialRotation, true);
    updateWidgets(false);
}

void MainWindow::on_frameIndex_valueChanged(int arg1)
{
    Engine::getInstance().setTime(arg1);
}

void MainWindow::on_restartFrame_clicked()
{
    ui->frameIndex->setValue(0);
}

void MainWindow::on_past_clicked()
{
    int currentFrame = ui->frameIndex->value();
    ui->frameIndex->setValue(currentFrame - 5);
}

void MainWindow::on_future_clicked()
{
    int currentFrame = ui->frameIndex->value();
    ui->frameIndex->setValue(currentFrame + 5);
}

void MainWindow::on_play_clicked()
{
    // Change button to notify user
    changeText(ui->play, "playing");

    Engine& engine = Engine::getInstance();
    CameraWindow& cam = CameraWindow::getInstance();
    IrrlichtDevice* device = cam.getDevice();
    // Calculate frametime in milliseconds from framerate
    int frametime = (1.0 / ((float)engine.getFramerate()))*1000;

    int from = ui->fromVideo->value();
    int to = ui->toVideo->value();
    QTime timer;
    for(int i = from; i <= to; ++i) {
        timer.restart();
        engine.setTime(i);

        // Wait frame time
        int remaining = frametime - timer.elapsed();
        if(remaining > 0)
            device->sleep(remaining);
    }

    // Restore current frame
    engine.setTime(ui->frameIndex->value());
    changeText(ui->play, "Play");
}

void MainWindow::changeText(QPushButton* button, const QString& text)
{
    button->setText(text);
    button->repaint();
}

void MainWindow::on_recordVideo_clicked()
{
    // Change button to notify user
    changeText(ui->recordVideo, "recording...");
    Engine& engine = Engine::getInstance();

    int from = ui->fromVideo->value();
    int to = ui->toVideo->value();
    int index = ui->frameIndex->value();

    // Save video and display encoding time
    QTime timer;
    timer.start();
    engine.saveVideo(from, to, index);
    std::cerr << "Time to create video : " << timer.elapsed()/1000.0 << std::endl;

    changeText(ui->recordVideo, "Record");
}

void MainWindow::on_takeScreenshot_clicked()
{
    // Take screenshot and name it with current time
    CameraWindow& cam = CameraWindow::getInstance();
    cam.takeScreenshot(QDateTime::currentDateTime().toTime_t());
}
