/**
  * 3D Avatars
  * Pierre Walch
  */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
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
 * This class represents another controller (regarding Engine) in MVC pattern. It allows the user
 * to control camera and animation with a GUI window.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

        /**
         * Updates widgets from Irrlicht window current settings and resets some settings if wanted
         */
        void updateWidgets();

    private slots:

        void on_xPos_valueChanged(double arg1);
        void on_yPos_valueChanged(double arg1);
        void on_zPos_valueChanged(double arg1);
        void on_xRot_valueChanged(double arg1);
        void on_yRot_valueChanged(double arg1);
        void on_zRot_valueChanged(double arg1);

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

        void keyPressEvent(QKeyEvent * e);

        void on_recordVideo_clicked();
        void on_frameIndex_valueChanged(int arg1);

        void on_past_clicked();
        void on_future_clicked();
        void on_play_clicked();

        void on_restartFrame_clicked();

        void on_takeScreenshot_clicked();

        void setFpsScale(double scale);

private:
        Ui::MainWindow *ui;

        // Initial values for the camera
        vector3df initialPosition;
        vector3df initialRotation;
        int initialSpeed;
        int initialTime;

        void changeText(QPushButton* button, const QString& text);
        void moveCamera(const vector3df& vector);
        void rotateCamera(const vector3df& vector);
        void setCameraPosition(const vector3df& vector, bool updateScene);
        void setCameraRotation(const vector3df& vector, bool updateScene);
        void blockAllSignals(bool state);
};

#endif // MAINWINDOW_H
