/**
  * 3D Avatars
  * Pierre Walch
  */

#ifndef ENGINE_H
#define ENGINE_H

#include <QApplication>
#include <vector>
#include "court.h"


/**
 * Handles user settings and interacts with Qt's window and Irrlicht's window
 */
class Engine
{
    public:
        /**
         * Returns singleton instance of the engine
         * @return instance of the engine
         */
        static Engine& getInstance();
        ~Engine();

        /**
         * Starts the engine and extracts settings from XML file
         * @param app corresponding Qt application
         * @return status
         */
        int start(const QApplication& app, const std::vector<std::string>& args);

        /**
         * Adapts the court to new time value
         * @param time new date
         */
        void setTime(int time);

        /**
         * Returns number of frames of the sampling
         * @return sampling frame number
         */
        int getFrameNumber() const;

        /**
         * Returns framerate
         * @return sampling framerate
         */
        int getFramerate() const;

        /**
         * Encodes a video from an initial frame to another frame, and saves it to a specified place
         * @param from begin frame
         * @param to end frame
         * @param beforeTime engine frame before encoding (to restore state)
         */
        void saveVideo(int from, int to, int beforeTime = -1);

        /**
         * Returns court
         * @return
         */
        Court* getCourt() const;

        /**
         * Returns current frame time
         * @return frame time
         */
        int getCurrentTime() const;

        int getStartTime() const;
        int getEndTime() const;

private:
        Engine() {}
        Engine& operator= (const Engine&) { }
        Engine(const Engine&) {}

        void loadSettings(const std::string& cfgPath);
        std::vector<float> getSplittenLine(const std::string& line);
        void parsingError(const std::string& msg);

        Court* court;
        int frameNumber;
        int framerate;
        int startTime;
        int endTime;
        std::string videoName;

        int currentTime;
        bool inConsole;

};

#endif // ENGINE_H
