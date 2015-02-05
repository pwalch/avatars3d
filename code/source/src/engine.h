/*
 *  Copyright 2014 Pierre Walch
 *  Website : www.pwalch.net
 *
 *  Avatars is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  Avatars is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with Avatars.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#include <QApplication>
//#include <X11/Xlib.h>
#include <vector>

#include "camerawindow.h"
#include "court.h"
#include "itimeable.h"
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
class Engine : public ITimeable
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
    virtual void setTime(int time) override;

    /**
     * Returns court containing players and ball trajectories
     * @return court
     */
    const Court& getCourt() const;

    /**
     * Returns 3D view
     * @return camera window
     */
    CameraWindow& getCameraWindow() const;

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
     * Plays the scene in the 3D view according to the framerate and a play interval.
     * It is interrupted by stopPlaying()
     * @see stopPlaying()
     * @see saveVideo()
     * @param from beginning of sub-sequence
     * @param to end of sub-sequence
     */
    void play(int from, int to);

    /**
     * Called from EventManager to stop recording a video
     * @see saveVideo()
     * @see EventManager
     */
    void stopRecording();

    /**
     * Called from Qt to stop playing a video
     * @see play()
     * @see MainWindow
     */
    void stopPlaying();

    /**
     * Called from Qt to stop playing a live sequence
     * @see playLive()
     * @see MainWindow
     */
    void stopLivePlaying();

    /**
     * Returns current frame
     * @return displayed frame
     */
    int getCurrentFrame() const;

    /**
     * Returns the affine transformation used by the program to perform coordinate conversions between
     * reality and Irrlicht
     * @return transformation
     */
    const AffineTransformation& getAffineTransformation() const;


private:

    // Singletons denials
    Engine();
    Engine& operator= (const Engine&);
    Engine(const Engine&);

    // Helper methods
    void loadSettings(const std::string& cfgPath);

    /**
     * Updates the trajectories by taking new frames from the streams
     * @param nbFramesToCatch number of new frames to take
     */
    void updateTrajectories(int nbFramesToCatch);

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
     * Retrieves data from the streams and plays it in real time.
     * Is interrupted by stopLivePlaying();
     * @see saveVideo()
     * @see stopLivePlaying();
     */
    void livePlay();




    std::unique_ptr<AvatarsFactory> mFactory;

    SequenceSettings mSequenceSettings;
    std::unique_ptr<AffineTransformation> mTransformation;

    int mCurrentFrame;
    std::unique_ptr<Court> mCourt;
    std::unique_ptr<CameraWindow> mCameraWindow;

    std::unique_ptr<std::istream> mCameraStream;
    std::unique_ptr<std::istream> mPlayerStream;
    std::unique_ptr<std::istream> mBallStream;

    // Video saving interruption flag
    bool mIsRecording;
    bool mIsPlaying;
    bool mIsLivePlaying;

};

#endif // ENGINE_H
