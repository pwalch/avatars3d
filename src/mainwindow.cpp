/*
  * 3D Avatars
  * Pierre Walch
  */

#include <iostream>
#include <fstream>
#include <irrlicht.h>
#include <QKeyEvent>
#include <QTime>
#include <QThread>

#include "camerawindow.h"
#include "engine.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"


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
    int frameNumber = engine.getSequenceSettings().mFrameNumber;

    // Set minimums and maximums for frame indexes
    mUi->frameIndex->setMinimum(0);
    mUi->frameIndex->setMaximum(frameNumber - 1);
    mUi->fromVideo->setMinimum(0);
    mUi->fromVideo->setMaximum(frameNumber - 2);
    mUi->toVideo->setMinimum(1);
    mUi->toVideo->setMaximum(frameNumber - 1);

    // Sets fpsScale of FPS camera
    int fpsScale = Engine::getInstance().getCameraWindow().getSettings().mFpsScale;
    mUi->fpsScale->setValue(fpsScale);

    // Initialize frame navigation widgets
    mUi->fromVideo->setValue(engine.getSequenceSettings().mStartTime);
    mUi->toVideo->setValue(engine.getSequenceSettings().mEndTime);
    modifyWithoutEvent(mUi->frameIndex, engine.getSequenceSettings().mInitialTime);

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
    CameraWindow& cam = Engine::getInstance().getCameraWindow();

    // Update position widgets
    auto position = cam.getRealPosition();
    modifyWithoutEvent(mUi->xPos, position.X);
    modifyWithoutEvent(mUi->yPos, position.Y);
    modifyWithoutEvent(mUi->zPos, position.Z);

    // Update rotation widgets
    auto rotation = cam.getRotation();
    modifyWithoutEvent(mUi->xRot, rotation.X);
    modifyWithoutEvent(mUi->yRot, rotation.Y);
    modifyWithoutEvent(mUi->zRot, rotation.Z);
}

void MainWindow::blockAnimationTab()
{
    mUi->useTrajectoryFile->setChecked(false);
    blockAnimationSignals(true);
}

void MainWindow::setFollowTrajectory(bool isFollowing)
{
    mUi->useTrajectoryFile->setChecked(isFollowing);
}

void MainWindow::setCameraRealPosition(const vector3df& vector)
{
    CameraWindow& cam = Engine::getInstance().getCameraWindow();
    cam.setRealPosition(vector);
    cam.updateScene();
}

void MainWindow::on_xPos_valueChanged(double arg1)
{
    auto currentPosition = Engine::getInstance().getCameraWindow().getRealPosition();
    currentPosition.X = (float)arg1;
    setCameraRealPosition(currentPosition);
}

void MainWindow::on_yPos_valueChanged(double arg1)
{
    auto currentPosition = Engine::getInstance().getCameraWindow().getRealPosition();
    currentPosition.Y = (float)arg1;
    setCameraRealPosition(currentPosition);
}

void MainWindow::on_zPos_valueChanged(double arg1)
{
    auto currentPosition = Engine::getInstance().getCameraWindow().getRealPosition();
    currentPosition.Z = (float)arg1;
    setCameraRealPosition(currentPosition);
}

void MainWindow::setCameraRotation(const vector3df& vector)
{
    CameraWindow& cam = Engine::getInstance().getCameraWindow();
    cam.setRotation(vector);
    cam.updateScene();
}

void MainWindow::on_xRot_valueChanged(double arg1)
{
    auto currentRotation = Engine::getInstance().getCameraWindow().getRotation();
    currentRotation.X = (float)arg1;
    setCameraRotation(currentRotation);
}

void MainWindow::on_yRot_valueChanged(double arg1)
{
    auto currentRotation = Engine::getInstance().getCameraWindow().getRotation();
    currentRotation.Y = (float)arg1;
    setCameraRotation(currentRotation);
}

void MainWindow::on_zRot_valueChanged(double arg1)
{
    vector3df currentRotation = Engine::getInstance().getCameraWindow().getRotation();
    currentRotation.Z = (float)arg1;
    setCameraRotation(currentRotation);
}


void MainWindow::moveCamera(const vector3df& virtualVector)
{
    CameraWindow& cam = Engine::getInstance().getCameraWindow();
    cam.moveVirtual(virtualVector);
    // Camera move changes position and rotation, so we update them in the UI
    updateWidgets();
    cam.updateScene();
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
    CameraWindow& cam = Engine::getInstance().getCameraWindow();
    cam.rotate(vector);
    updateWidgets();
    cam.updateScene();
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
        case Qt::Key_W: {
            on_forwardPos_clicked();
            break;
        }

        case Qt::Key_S: {
            on_backwardsPos_clicked();
            break;
        }

        case Qt::Key_A: {
            on_leftPos_clicked();
            break;
        }

        case Qt::Key_D: {
            on_rightPos_clicked();
            break;
        }

        case Qt::Key_E: {
            on_upPos_clicked();
            break;
        }

        case Qt::Key_C: {
            on_downPos_clicked();
            break;
        }

        case Qt::Key_I: {
            on_upRot_clicked();
            break;
        }

        case Qt::Key_K: {
            on_downRot_clicked();
            break;
        }

        case Qt::Key_J: {
            on_leftRot_clicked();
            break;
        }

        case Qt::Key_L: {
            on_rightRot_clicked();
            break;
        }

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
            Engine::getInstance().stopPlaying();
            Engine::getInstance().stopLivePlaying();
            break;
        }

        default: {
            break;
        }

    }
}

void MainWindow::on_frameIndex_valueChanged(int arg1)
{
    Engine::getInstance().setTime(arg1);
    updateWidgets();
}

void MainWindow::on_restartFrame_clicked()
{
    mUi->frameIndex->setValue(Engine::getInstance().getSequenceSettings().mInitialTime);
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

    int from = mUi->fromVideo->value();
    int to = mUi->toVideo->value();

    int beforeTime = engine.getCurrentFrame();
    engine.play(from, to);
    engine.setTime(beforeTime);

    changeText(mUi->play, "Play");
    blockAnimationSignals(false);
}

void MainWindow::changeText(QPushButton* button, const QString& text)
{
    button->setText(text);
    button->repaint();
}

void MainWindow::on_takeScreenshot_clicked()
{
    // Take screenshot and name it with current time
    Engine::getInstance().getCameraWindow().takeScreenshot(QDateTime::currentDateTime().toTime_t());
}

void MainWindow::on_useTrajectoryFile_clicked()
{
    Engine::getInstance().getCameraWindow().setFollowTrajectoryFile(mUi->useTrajectoryFile->isChecked());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Engine::getInstance().stopLivePlaying();
    Engine::getInstance().stopPlaying();
}
