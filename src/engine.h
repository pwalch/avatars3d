/**
  * 3D Avatars
  * Pierre Walch
  */

#ifndef ENGINE_H
#define ENGINE_H

#include <QApplication>
//#include <X11/Xlib.h>
#include <vector>
#include "court.h"
#include "timeable.h"

/**
 * @brief Handles time and interacts with Qt window and Irrlicht window (controller)
 *
 * This class represents controller in MVC pattern. It parses the settings file, controls time and saves videos.
 */
class Engine : public Timeable
{
    public:
        /**
         * Returns singleton instance of the engine
         * @return instance of the engine
         */
        static Engine& getInstance();
        virtual ~Engine();

        /**
         * Starts the engine and extracts settings from XML file
         * @param app corresponding Qt application
         * @param args list of arguments for the program
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
         * @return current date
         */
        int getCurrentTime() const;

        /**
         * Returns start time of the sequence
         * @return start date
         */
        int getStartTime() const;

        /**
         * Returns end time of the sequence
         * @return end date
         */
        int getEndTime() const;

        void throwError(const std::string& msg);

private:
        Engine() {}
        Engine& operator= (const Engine&) { }
        Engine(const Engine&) {}

        // Helper methods
        void loadSettings(const std::string& cfgPath);
        std::vector<float> getSplittenLine(const std::string& line);

        Court* court;

        // Input-output attributes
        bool inConsole;
        std::string videoName;

        // Time related attributes
        int frameNumber;
        int framerate;
        int startTime;
        int endTime;
        int currentTime;

};

#endif // ENGINE_H
