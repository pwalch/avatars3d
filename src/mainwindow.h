/*
  * 3D Avatars
  * Pierre Walch
  */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QMainWindow>
#include <irrlicht.h>
#include <QKeyEvent>
#include <QPushButton>

using namespace irr;
using namespace irr::core;

namespace Ui {
    class MainWindow;
}

/**
 * @brief Qt window, handling widget events (controller)
 *
 * Allows the user to control camera and animation with a Qt GUI window.
 */
class MainWindow : public QMainWindow
{

Q_OBJECT

public:

    /**
     * Initializes the widgets by using SequenceSettings object from Engine
     * @param parent parent widget
     */
    explicit MainWindow(QWidget *parent = 0);

    /**
     * Releases memory of Qt GUI
     */
    virtual ~MainWindow();

    /**
     * Updates widgets from Irrlicht window current settings and
     * resets some settings if wanted
     */
    void updateWidgets();

    /**
     * Blocks animation tab widgets
     */
    void blockAnimationTab();

private slots:

    void keyPressEvent(QKeyEvent * e);

    void on_xPos_valueChanged(double arg1);
    void on_yPos_valueChanged(double arg1);
    void on_zPos_valueChanged(double arg1);
    void on_xRot_valueChanged(double arg1);
    void on_yRot_valueChanged(double arg1);
    void on_zRot_valueChanged(double arg1);

    void setFpsScale(double scale);
    void on_forwardPos_clicked();
    void on_leftPos_clicked();
    void on_rightPos_clicked();
    void on_backwardsPos_clicked();
    void on_upPos_clicked();
    void on_downPos_clicked();
    void on_upRot_clicked();
    void on_leftRot_clicked();
    void on_rightRot_clicked();
    void on_downRot_clicked();

    void on_frameIndex_valueChanged(int arg1);
    void on_past_clicked();
    void on_future_clicked();
    void on_play_clicked();
    void on_restartFrame_clicked();

    void on_takeScreenshot_clicked();
    void on_useTrajectoryFile_clicked();

    void closeEvent(QCloseEvent *event);



private:

    /**
     * Changes text of a button
     * @param button button to modify
     * @param text new text
     */
    void changeText(QPushButton* button, const QString& text);

    /**
     * Moves the camera by a virtual move vector, then updates the 3D view and the Qt widgets
     * @param virtualVector virtual move vector
     */
    void moveCamera(const vector3df& virtualVector);

    /**
     * Rotates camera by rotation vector, then updates the 3D view and the Qt widgets
     * @param vector rotation vector
     */
    void rotateCamera(const vector3df& vector);

    /**
     * Sets camera real position and updates 3D view
     * @param realVector real vector
     */
    void setCameraRealPosition(const vector3df& realVector);

    /**
     * Sets camera rotation and updates 3D view
     * @param vector rotation vector
     */
    void setCameraRotation(const vector3df& vector);

    /**
     * If isBlocked is true, the widget becomes event-blocked and disabled. If isBlocked is false, the widget
     * becomes event-activated and enabled.
     * @param w widget to modify
     * @param isBlocked true if blocked, false else
     */
    void modifyBlockingState(QWidget* w, bool isBlocked);

    /**
     * If isBlocked is true, blocks all the widgets of numeric values (position, rotation, FPS scale).
     * @param isBlocked true if blocked, false else
     */
    void blockNumericValuesSignals(bool isBlocked);

    /**
     * If isBlocked is true, blocks all the widgets of animation (animation tab widgets)
     * @param isBlocked true if blocked, false else
     */
    void blockAnimationSignals(bool isBlocked);

    /**
     * If is blocked is true, blocks all the widgets of FPS camera (relative move and rotation widgets)
     * @param isBlocked true if blocked, false else
     */
    void blockFPSCameraSignals(bool isBlocked);

    /**
     * Modifies a double spinbox without triggering an event
     * @param spinBox spinbox to modify
     * @param val float to put in the spinbox
     */
    void modifyWithoutEvent(QDoubleSpinBox *spinBox, double val);

    /**
     * Modifies an integer spinbox without triggering an event
     * @param spinBox spinbox to modify
     * @param val integer to put in the spinbox
     */
    void modifyWithoutEvent(QSpinBox *spinBox, double val);

    Ui::MainWindow* mUi;
};

#endif // MAINWINDOW_H
