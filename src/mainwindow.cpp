/*
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

    // Initialize sequence
    mInitialTime = engine.getSequenceSettings().mCurrentTime;
    int frameNumber = engine.getSequenceSettings().mFrameNumber;
    mIsPlaying = false;

    // Set minimums and maximums for frame indexes
    mUi->frameIndex->setMinimum(0);
    mUi->frameIndex->setMaximum(frameNumber - 1);
    mUi->fromVideo->setMinimum(0);
    mUi->fromVideo->setMaximum(frameNumber - 2);
    mUi->toVideo->setMinimum(1);
    mUi->toVideo->setMaximum(frameNumber - 1);

    // Sets fpsScale of FPS camera
    int fpsScale = CameraWindow::getInstance()->getSettings().mFpsScale;
    mUi->fpsScale->setValue(fpsScale);

    // Initialize frame navigation widgets
    mUi->fromVideo->setValue(engine.getSequenceSettings().mStartTime);
    mUi->toVideo->setValue(engine.getSequenceSettings().mEndTime);
    modifyWithoutEvent(mUi->frameIndex, engine.getSequenceSettings().mCurrentTime);

    updateWidgets();
}

MainWindow::~MainWindow()
{
    delete mUi;
}

void MainWindow::setFpsScale(double val)
{
    modifyWithoutEvent(mUi->fpsScale, val);
}

void MainWindow::updateWidgets()
{
    CameraWindow* cam = CameraWindow::getInstance();

    // Update position widgets
    vector3df position = cam->getRealPosition();
    modifyWithoutEvent(mUi->xPos, position.X);
    modifyWithoutEvent(mUi->yPos, position.Y);
    modifyWithoutEvent(mUi->zPos, position.Z);

    // Update rotation widgets
    vector3df rotation = cam->getRotation();
    modifyWithoutEvent(mUi->xRot, rotation.X);
    modifyWithoutEvent(mUi->yRot, rotation.Y);
    modifyWithoutEvent(mUi->zRot, rotation.Z);
}

void MainWindow::setCameraRealPosition(const vector3df& vector)
{
    CameraWindow* cam = CameraWindow::getInstance();
    cam->setRealPosition(vector);
    cam->updateScene();
}

void MainWindow::on_xPos_valueChanged(double arg1)
{
    vector3df currentPosition = CameraWindow::getInstance()->getRealPosition();
    currentPosition.X = (float)arg1;
    setCameraRealPosition(currentPosition);
}

void MainWindow::on_yPos_valueChanged(double arg1)
{
    vector3df currentPosition = CameraWindow::getInstance()->getRealPosition();
    currentPosition.Y = (float)arg1;
    setCameraRealPosition(currentPosition);
}

void MainWindow::on_zPos_valueChanged(double arg1)
{
    vector3df currentPosition = CameraWindow::getInstance()->getRealPosition();
    currentPosition.Z = (float)arg1;
    setCameraRealPosition(currentPosition);
}

void MainWindow::setCameraRotation(const vector3df& vector)
{
    CameraWindow* cam = CameraWindow::getInstance();
    cam->setRotation(vector);
    cam->updateScene();
}

void MainWindow::on_xRot_valueChanged(double arg1)
{
    vector3df currentRotation = CameraWindow::getInstance()->getRotation();
    currentRotation.X = (float)arg1;
    setCameraRotation(currentRotation);
}

void MainWindow::on_yRot_valueChanged(double arg1)
{
    vector3df currentRotation = CameraWindow::getInstance()->getRotation();
    currentRotation.Y = (float)arg1;
    setCameraRotation(currentRotation);
}

void MainWindow::on_zRot_valueChanged(double arg1)
{
    vector3df currentRotation = CameraWindow::getInstance()->getRotation();
    currentRotation.Z = (float)arg1;
    setCameraRotation(currentRotation);
}


void MainWindow::moveCamera(const vector3df& virtualVector)
{
    CameraWindow* cam = CameraWindow::getInstance();
    cam->moveVirtual(virtualVector);
    // Camera move changes position and rotation, so we update them in the UI
    updateWidgets();
    cam->updateScene();
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

void MainWindow::rotateCamera(const vector3df& vector)
{
    CameraWindow* cam = CameraWindow::getInstance();
    cam->rotate(vector);
    updateWidgets();
    cam->updateScene();
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


void MainWindow::blockNumericValuesSignals(bool isBlocked)
{
    modifyBlockingState(mUi->xPos, isBlocked);
    modifyBlockingState(mUi->yPos, isBlocked);
    modifyBlockingState(mUi->zPos, isBlocked);

    modifyBlockingState(mUi->xRot, isBlocked);
    modifyBlockingState(mUi->yRot, isBlocked);
    modifyBlockingState(mUi->zRot, isBlocked);

    modifyBlockingState(mUi->fpsScale, isBlocked);
}

void MainWindow::blockAnimationSignals(bool isBlocked) {
    modifyBlockingState(mUi->play, isBlocked);
    modifyBlockingState(mUi->recordVideo, isBlocked);
    modifyBlockingState(mUi->frameIndex, isBlocked);
    modifyBlockingState(mUi->past, isBlocked);
    modifyBlockingState(mUi->future, isBlocked);
    modifyBlockingState(mUi->restartFrame, isBlocked);
    modifyBlockingState(mUi->useTrajectoryFile, isBlocked);
    modifyBlockingState(mUi->fromVideo, isBlocked);
    modifyBlockingState(mUi->toVideo, isBlocked);

    bool followTrajectoryFile = mUi->useTrajectoryFile->isChecked();
    if(followTrajectoryFile) {
        blockNumericValuesSignals(isBlocked);
        blockFPSCameraSignals(isBlocked);
    }
}

void MainWindow::blockFPSCameraSignals(bool isBlocked) {
    modifyBlockingState(mUi->forwardPos, isBlocked);
    modifyBlockingState(mUi->backwardsPos, isBlocked);
    modifyBlockingState(mUi->leftPos, isBlocked);
    modifyBlockingState(mUi->rightPos, isBlocked);
    modifyBlockingState(mUi->upPos, isBlocked);
    modifyBlockingState(mUi->downPos, isBlocked);

    modifyBlockingState(mUi->leftRot, isBlocked);
    modifyBlockingState(mUi->rightRot, isBlocked);
    modifyBlockingState(mUi->upRot, isBlocked);
    modifyBlockingState(mUi->downRot, isBlocked);
}

void MainWindow::modifyWithoutEvent(QDoubleSpinBox *spinBox, double val)
{
    modifyBlockingState(spinBox, true);
    spinBox->setValue(val);
    modifyBlockingState(spinBox, false);
}

void MainWindow::modifyWithoutEvent(QSpinBox *spinBox, double val)
{
    modifyBlockingState(spinBox, true);
    spinBox->setValue(val);
    modifyBlockingState(spinBox, false);
}


void MainWindow::modifyBlockingState(QWidget* w, bool isBlocked) {
    w->blockSignals(isBlocked);
    w->setEnabled(!isBlocked);
}

void MainWindow::keyPressEvent(QKeyEvent * e)
{
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

        case Qt::Key_U: {
            double val = mUi->fpsScale->value();
            setFpsScale(val + 1);
            break;
        }

        case Qt::Key_M: {
            double val = mUi->fpsScale->value();
            setFpsScale(val - 1);
            break;
        }

        case::Qt::Key_O: {
            on_past_clicked();
            break;
        }

        case::Qt::Key_P: {
            on_future_clicked();
            break;
        }

        case Qt::Key_Escape: {
            mIsPlaying = false;
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
    blockAnimationSignals(true);
    changeText(mUi->play, "ESCAPE key to stop");

    Engine& engine = Engine::getInstance();
    CameraWindow* cam = CameraWindow::getInstance();
    IrrlichtDevice* device = cam->getDevice();

    // Calculate frametime in milliseconds from framerate
    int frametime = (1.0 / ((float)engine.getSequenceSettings().mFramerate)) * 1000;

    int from = mUi->fromVideo->value();
    int to = mUi->toVideo->value();

    // Save time value to restore state after playing
    int beforeTime = mUi->frameIndex->value();
    QTime timer;
    mIsPlaying = true;
    for(int i = from; i <= to; ++i) {
        timer.restart();
        engine.setTime(i);

        // Wait frame time
        int remaining = frametime - timer.elapsed();
        if(remaining > 0)
            device->sleep(remaining);

        // Stops to play to process events and check for interruption
        QApplication::processEvents();
        if(!mIsPlaying) {
            break;
        }
    }
    engine.setTime(beforeTime);
    mIsPlaying = false;

    changeText(mUi->play, "Play");
    blockAnimationSignals(false);
}

void MainWindow::changeText(QPushButton* button, const QString& text)
{
    button->setText(text);
    button->repaint();
}

void MainWindow::on_recordVideo_clicked()
{
    blockAnimationSignals(true);
    modifyBlockingState(mUi->takeScreenshot, true);

    changeText(mUi->recordVideo, "recording...");
    Engine& engine = Engine::getInstance();

    int from = mUi->fromVideo->value();
    int to = mUi->toVideo->value();
    int current = mUi->frameIndex->value();

    // Actually Save video
    QTime timer;
    timer.start();
    engine.saveVideo(from, to, current);
    //std::cerr << "Time to create video : " << timer.elapsed()/1000.0 << std::endl;

    changeText(mUi->recordVideo, "Record");
    modifyBlockingState(mUi->takeScreenshot, false);
    blockAnimationSignals(false);
}

void MainWindow::on_takeScreenshot_clicked()
{
    // Take screenshot and name it with current time
    CameraWindow* cam = CameraWindow::getInstance();
    cam->takeScreenshot(QDateTime::currentDateTime().toTime_t());
}

void MainWindow::on_useTrajectoryFile_clicked()
{
    CameraWindow* cam = CameraWindow::getInstance();
    cam->setFollowTrajectoryFile(mUi->useTrajectoryFile->isChecked());
}
