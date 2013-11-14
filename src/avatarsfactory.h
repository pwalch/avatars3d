/*
  * 3D Avatars
  * Pierre Walch
  */


#ifndef AVATARSFACTORY_H
#define AVATARSFACTORY_H

#include <iostream>
#include "libs/tinyxml2.h"
#include "camerasettings.h"
#include "engine.h"
#include "court.h"
#include "settingsparser.h"

using namespace tinyxml2;

/**
 * @brief Factory for common objects of the program
 *
 * Creates the objects used by Engine. To achieve this, the factory retrieves raw data from a SettingsParser instance.
 */
class AvatarsFactory
{
    public:

        /**
         * Creates the parsing system and saves shortcuts of the main nodes and their subnodes
         * @param cfgPath configuration file to parse
         */
        AvatarsFactory(std::string cfgPath);

        /**
         * Releases memory of the SettingsParser instance
         */
        virtual ~AvatarsFactory();

        /**
         * Returns an initialized court according to configuration file
         * @return initialized court
         */
        Court* createCourt();

        /**
         * Returns sequence settings
         * @return sequence settings
         */
        SequenceSettings retrieveSequenceSettings();

        /**
         * Creates an affine transformation by retrieving its settings from configuration file
         * @return pointer to affine transformation
         */
        AffineTransformation* createTransformation();

        /**
         * Creates camera window instance and initializes it
         */
        void constructCamera();


    private:
        SettingsParser* mSettingsParser;

        MovingBody* createBall();
        std::map<int, Player*> createPlayerMap();

};

#endif // AVATARSFACTORY_H
