#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <irrlicht.h>
#include <QKeyEvent>
#include <QPushButton>

namespace Ui {
    class MainWindow;
}

/**
 * Qt window, handling widget events
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

        /**
         * Updates widgets from Irrlicht window current settings and resets initial position if wanted
         * @param reset specifies whether the initial camera position must be reset or not
         */
        void updateWidgets(bool reset);

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


        void on_resetCamera_clicked();
        void on_recordVideo_clicked();
        void on_frameIndex_valueChanged(int arg1);

        void on_past_clicked();
        void on_future_clicked();
        void on_play_clicked();

        void on_restartFrame_clicked();

        void on_takeScreenshot_clicked();

private:
        Ui::MainWindow *ui;
        irr::core::vector3df initialPosition;
        irr::core::vector3df initialRotation;
        int initialSpeed;

        void changeText(QPushButton* button, const QString& text);
        void moveCamera(irr::core::vector3df vector);
        void rotateCamera(irr::core::vector3df vector);
        void setCameraPosition(irr::core::vector3df vector, bool updateScene);
        void setCameraRotation(irr::core::vector3df vector, bool updateScene);
};

#endif // MAINWINDOW_H
