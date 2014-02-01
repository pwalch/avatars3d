/*
  * 3D Avatars
  * Pierre Walch
  */

#ifndef ENGINE_H
#define ENGINE_H

#include <QApplication>
//#include <X11/Xlib.h>
#include <vector>

#include "camerawindow.h"
#include "court.h"
#include "timeable.h"
#include "affinetransformation.h"
#include "sequencesettings.h"
#include "avatarsfactory.h"


class AvatarsFactory;

/**
 * @brief Controls the trajectory data and Irrlicht window (controller)
 *
 * Entry point of the program. start() must be called directly after instanciation.
 *
 */
class Engine : public Timeable
{
    public:
        /**
         * Returns singleton instance of the engine
         * @return instance of the engine
         */
        static Engine& getInstance();

        /**
         * Releases memory for trajectories and affine transformation
         */
        virtual ~Engine();

        /**
         * Starts the engine. Must be called after instanciation, before any other method
         * @param app corresponding Qt application
         * @param args list of arguments for the program
         * @return status code
         */
        int start(const QApplication& app, const std::vector<std::string>& args);

        /**
         * Moves the players, the ball and the camera to the corresponding position/rotation
         * @param time time index
         * @see CameraWindow::setTime()
         * @see Court::setTime()
         */
        void setTime(int time);

        /**
         * Returns court containing players and ball trajectories
         * @return court
         */
        Court* getCourt() const;

        /**
         * Returns 3D view
         * @return camera window
         */
        CameraWindow* getCameraWindow() const;

        /**
         * Quits program with status code 1, and displays error message
         * @param errorMessage error message to display
         */
        void throwError(const stringw& errorMessage);

        /**
         * Returns sequence settings
         * @see SequenceSettings
         * @return sequence settings
         */
        const SequenceSettings& getSequenceSettings() const;

        /**
         * Plays the scene in the 3D view according to framerate and a play interval
         * @param fromFrame beginning of sub-sequence
         * @param toFrame end of sub-sequence
         */
        void play(int fromFrame, int toFrame);

        /**
         * Encodes a video from an initial frame to another frame, and saves it to the place specified in
         * CameraWindow settings. The encoding continues until the whole sequence has been processed, or
         * until the process is interrupted. During encoding, the method perodically checks whether new Irrlicht
         * window events have been thrown. If so, the event manager processes them and is therefore capable
         * of interrupting the process by calling stopRecording().
         * @see CameraSettings
         * @see EventManager
         * @see stopRecording()
         * @param from begin frame
         * @param to end frame
         */
        void saveVideo(int from, int to);

        /**
         * Called from EventManager to stop recording a video
         * @see saveVideo()
         * @see EventManager
         */
        void stopRecording();

        /**
         * Called from Qt to stop playing a video
         * @see play()
         * @see EventManager
         */
        void stopPlaying();

        /**
         * Returns the affine transformation used by the program to perform coordinate conversions between
         * reality and Irrlicht
         * @return transformation
         */
        AffineTransformation *getTransformation() const;

private:
        // Singletons denials
        Engine();
        Engine& operator= (const Engine&) { return getInstance(); }
        Engine(const Engine&) { }

        // Helper methods
        void loadSettings(const std::string& cfgPath);

        void updateTrajectories(int nbFramesToCatch);

        AvatarsFactory* mFactory;

        SequenceSettings mSequenceSettings;
        AffineTransformation* mTransformation;

        int mCurrentFrame;
        Court* mCourt;
        CameraWindow* mCameraWindow;

        std::istream* mCameraStream;
        std::istream* mPlayerStream;
        std::istream* mBallStream;

        // Video saving interruption flag
        bool mIsRecording;
        bool mIsPlaying;

};

#endif // ENGINE_H
