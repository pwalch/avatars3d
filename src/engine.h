#ifndef ENGINE_H
#define ENGINE_H

#include <QApplication>
#include <vector>
#include "court.h"

/**
 * The singleton engine handles user settings and interacts with Qt's window and Irrlicht's window
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
        int start(const QApplication& app);

        /**
         * Adapts the court to new time value
         * @param time new date
         */
        void setTime(const int time);

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
         * @param currentFrame engine frame before encoding (to restore state)
         * @param name output filename
         */
        void saveVideo(const int from, const int to, const int currentFrame);

private:
        Engine() {}
        Engine& operator= (const Engine&) { }
        Engine(const Engine&) {}

        void loadSettings();
        std::vector<int> getSplittenLine(std::string line);

        Court* court;
        int frameNumber;
        int framerate;
        std::string videoName;

};

#endif // ENGINE_H
