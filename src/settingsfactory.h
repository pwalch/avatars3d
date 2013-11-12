/**
  * 3D Avatars
  * Pierre Walch
  */


#ifndef SETTINGSFACTORY_H
#define SETTINGSFACTORY_H

#include <iostream>
#include "libs/tinyxml2.h"
#include "camerasettings.h"
#include "engine.h"
#include "court.h"
#include "configurationfileparser.h"

using namespace tinyxml2;

/**
 * @brief Factory for common objects of the program
 *
 * Initializes the program with settings from XML file.
 */
class SettingsFactory
{
    public:
        /**
         * Creates the parsing system and saves shortcuts of the main nodes
         * @param cfgPath configuration file to parse
         */
        SettingsFactory(std::string cfgPath);
        virtual ~SettingsFactory();

        /**
         * Returns an initialized court according to configuration file
         * @return court
         */
        Court* createCourt();

        /**
         * Returns sequence settings
         * @return sequence settings
         */
        SequenceSettings retrieveSequenceSettings();

        AffineTransformation* createAffineTransformation();

        /**
         * Initializes CameraWindow
         */
        void constructCamera();


    private:
        ConfigurationFileParser* mConfigurationFileParser;
        MovingBody* createBall();
        std::map<int, Player*> createPlayerMap();

};

#endif // SETTINGSFACTORY_H
