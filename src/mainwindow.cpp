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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
            mUi(new Ui::MainWindow)
{
    // Qt window title
    mUi->setupUi(this);
    this->setWindowTitle("Avatars controller");

    Engine& engine = Engine::getInstance();
    mInitialTime = engine.getSequenceSettings().mCurrentTime;
    int frameNumber = engine.getSequenceSettings().mFrameNumber;
    mPlayVideo = false;

    // Set minimums and maximums
    mUi->frameIndex->setMinimum(0);
    mUi->frameIndex->setMaximum(frameNumber - 1);
    mUi->fromVideo->setMinimum(0);
    mUi->fromVideo->setMaximum(frameNumber - 2);
    mUi->toVideo->setMinimum(1);
    mUi->toVideo->setMaximum(frameNumber - 1);

    // Sets fpsScale of FPS camera
    int fpsScale = CameraWindow::getInstance().getSettings().mFpsScale;
    mUi->fpsScale->setValue(fpsScale);

    updateWidgets();
}

MainWindow::~MainWindow()
{
    delete mUi;
}

void MainWindow::updateWidgets()
{
    // Avoid updating the scene each time a widget is modified
    blockAllSignals(true);

    CameraWindow& cam = CameraWindow::getInstance();

    // Update position widgets
    vector3df position = cam.getRealPosition();
    mUi->xPos->setValue(position.X);
    mUi->yPos->setValue(position.Y);
    mUi->zPos->setValue(position.Z);

    // Update rotation widgets
    vector3df rotation = cam.getRotation();
    mUi->xRot->setValue(rotation.X);
    mUi->yRot->setValue(rotation.Y);
    mUi->zRot->setValue(rotation.Z);

    // Update frame navigation widgets
    Engine& engine = Engine::getInstance();
    int currentTime = engine.getSequenceSettings().mCurrentTime;
    int startTime = engine.getSequenceSettings().mStartTime;
    int endTime = engine.getSequenceSettings().mEndTime;

    mUi->fromVideo->setValue(startTime);
    mUi->toVideo->setValue(endTime);
    mUi->frameIndex->setValue(currentTime);

    blockAllSignals(false);
}

void MainWindow::setCameraPosition(const vector3df& vector, bool updateScene)
{
    CameraWindow& cam = CameraWindow::getInstance();
    cam.setRealPosition(vector);
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

void MainWindow::blockAllSignals(bool state)
{
    mUi->xPos->blockSignals(state);
    mUi->yPos->blockSignals(state);
    mUi->zPos->blockSignals(state);
    mUi->xRot->blockSignals(state);
    mUi->yRot->blockSignals(state);
    mUi->zRot->blockSignals(state);

    mUi->frameIndex->blockSignals(state);
}

void MainWindow::moveCamera(const vector3df& vector)
{
    CameraWindow& cam = CameraWindow::getInstance();
    cam.move(vector);
    // Camera move changes position and rotation, so we update them in the UI
    updateWidgets();
    cam.updateScene();
}

void MainWindow::rotateCamera(const vector3df& vector)
{
    CameraWindow& cam = CameraWindow::getInstance();
    cam.rotate(vector);
    updateWidgets();
    cam.updateScene();
}

void MainWindow::on_xPos_valueChanged(double arg1)
{
    vector3df currentPosition = CameraWindow::getInstance().getRealPosition();
    currentPosition.X = (float)arg1;
    setCameraPosition(currentPosition, true);
}

void MainWindow::on_yPos_valueChanged(double arg1)
{
    vector3df currentPosition = CameraWindow::getInstance().getRealPosition();
    currentPosition.Y = (float)arg1;
    setCameraPosition(currentPosition, true);
}

void MainWindow::on_zPos_valueChanged(double arg1)
{
    vector3df currentPosition = CameraWindow::getInstance().getRealPosition();
    currentPosition.Z = (float)arg1;
    setCameraPosition(currentPosition, true);
}

void MainWindow::on_xRot_valueChanged(double arg1)
{
    vector3df currentRotation = CameraWindow::getInstance().getRotation();
    currentRotation.X = (float)arg1;
    setCameraRotation(currentRotation, true);
}

void MainWindow::on_yRot_valueChanged(double arg1)
{
    vector3df currentRotation = CameraWindow::getInstance().getRotation();
    currentRotation.Y = (float)arg1;
    setCameraRotation(currentRotation, true);
}

void MainWindow::on_zRot_valueChanged(double arg1)
{
    vector3df currentRotation = CameraWindow::getInstance().getRotation();
    currentRotation.Z = (float)arg1;
    setCameraRotation(currentRotation, true);
}

void MainWindow::on_forwardPos_clicked()
{
    moveCamera(vector3df(this->mUi->fpsScale->value(), 0, 0));
}

void MainWindow::on_backwardsPos_clicked()
{
    moveCamera(vector3df(-this->mUi->fpsScale->value(), 0, 0));
}

void MainWindow::on_leftPos_clicked()
{
    moveCamera(vector3df(0, this->mUi->fpsScale->value(), 0));
}

void MainWindow::on_rightPos_clicked()
{
    moveCamera(vector3df(0, -this->mUi->fpsScale->value(), 0));
}

void MainWindow::on_upPos_clicked()
{
    moveCamera(vector3df(0, 0, this->mUi->fpsScale->value()));
}

void MainWindow::on_downPos_clicked()
{
    moveCamera(vector3df(0, 0, -this->mUi->fpsScale->value()));
}

void MainWindow::on_upRot_clicked()
{
    rotateCamera(vector3df(this->mUi->fpsScale->value(), 0, 0));
}

void MainWindow::on_downRot_clicked()
{
    rotateCamera(vector3df(-this->mUi->fpsScale->value(), 0, 0));
}

void MainWindow::on_leftRot_clicked()
{
    rotateCamera(vector3df(0, this->mUi->fpsScale->value(), 0));
}

void MainWindow::on_rightRot_clicked()
{
    rotateCamera(vector3df(0, -this->mUi->fpsScale->value(), 0));
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

        case Qt::Key_Up: {
            double val = mUi->fpsScale->value();
            setFpsScale(val + 1);
            break;
        }

        case Qt::Key_Down: {
            double val = mUi->fpsScale->value();
            setFpsScale(val - 1);
            break;
        }

        case Qt::Key_Escape: {
            mPlayVideo = false;
            break;
        }

        default:
            break;
    }
}

void MainWindow::on_frameIndex_valueChanged(int arg1)
{
    Engine::getInstance().setTime(arg1);
    updateWidgets();
}

void MainWindow::on_restartFrame_clicked()
{
    mUi->frameIndex->setValue(mInitialTime);
}

void MainWindow::on_past_clicked()
{
    int currentFrame = mUi->frameIndex->value();
    mUi->frameIndex->setValue(currentFrame - 5);
}

void MainWindow::on_future_clicked()
{
    int currentFrame = mUi->frameIndex->value();
    mUi->frameIndex->setValue(currentFrame + 5);
}

void MainWindow::on_play_clicked()
{
    // Change button to notify user
    changeText(mUi->play, "ESCAPE key to stop");

    Engine& engine = Engine::getInstance();
    CameraWindow& cam = CameraWindow::getInstance();
    IrrlichtDevice* device = cam.getDevice();

    // Calculate frametime in milliseconds from framerate
    int frametime = (1.0 / ((float)engine.getSequenceSettings().mFramerate))
                    * 1000;

    int from = mUi->fromVideo->value();
    int to = mUi->toVideo->value();

    QTime timer;
    mPlayVideo = true;
    for(int i = from; i <= to; ++i) {
        QApplication::processEvents();
        if(!mPlayVideo) {
            break;
        }

        timer.restart();
        engine.setTime(i);

        // Wait frame time
        int remaining = frametime - timer.elapsed();
        if(remaining > 0)
            device->sleep(remaining);
    }

    // Restore current frame
    engine.setTime(mUi->frameIndex->value());
    changeText(mUi->play, "Play");
}

void MainWindow::changeText(QPushButton* button, const QString& text)
{
    button->setText(text);
    button->repaint();
}

void MainWindow::on_recordVideo_clicked()
{
    // Change button to notify user
    changeText(mUi->recordVideo, "recording...");
    Engine& engine = Engine::getInstance();

    int from = mUi->fromVideo->value();
    int to = mUi->toVideo->value();
    int index = mUi->frameIndex->value();

    // Save video and display encoding time
    QTime timer;
    timer.start();
    engine.saveVideo(from, to, index);
    //std::cerr << "Time to create video : " <<
    // timer.elapsed()/1000.0 << std::endl;

    changeText(mUi->recordVideo, "Record");
}

void MainWindow::on_takeScreenshot_clicked()
{
    // Take screenshot and name it with current time
    CameraWindow& cam = CameraWindow::getInstance();
    cam.takeScreenshot(QDateTime::currentDateTime().toTime_t());
}

void MainWindow::setFpsScale(double scale)
{
    blockSignals(true);
    mUi->fpsScale->setValue(scale);
    blockSignals(false);
}

void MainWindow::on_useTrajectoryFile_clicked()
{
    CameraWindow& cam = CameraWindow::getInstance();
    cam.setUseTrajectoryFile(mUi->useTrajectoryFile->isChecked());
}
