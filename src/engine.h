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
#include "affinetransformation.h"
#include "sequencesettings.h"

/**
 * @brief Handles time and interacts with Qt window and Irrlicht
 *  window (controller)
 *
 * This class represents controller in MVC pattern.
 * It parses the settings file, controls time and saves videos.
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
        int start(const QApplication& app,
                  const std::vector<std::string>& args);

        /**
         * Adapts the court to new time value
         * @param time new date
         */
        void setTime(int time);

        /**
         * Encodes a video from an initial frame to another frame, and saves
         * it to a specified place
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
         * Quits program with return status 1, and displays error message
         * @param msg error message to display
         */
        void throwError(const std::string& msg);

        /**
         * Returns transformation from reality to Irrlicht
         * @return transformation
         */
        AffineTransformation* getTransformation() const;

        /**
         * Returns sequence settings
         * @return sequence settings
         */
        const SequenceSettings& getSequenceSettings() const;

private:
        Engine() {}
        Engine& operator= (const Engine&) { }
        Engine(const Engine&) {}

        // Helper methods
        void loadSettings(const std::string& cfgPath);
        std::vector<float> getSplittenLine(const std::string& line);

        SequenceSettings sequenceSettings;

        Court* court;
        AffineTransformation* transformation;

};

#endif // ENGINE_H
