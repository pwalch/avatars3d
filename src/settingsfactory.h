#ifndef SETTINGSFACTORY_H
#define SETTINGSFACTORY_H

#include <iostream>
#include "libs/tinyxml2.h"
#include "camerasettings.h"
#include "engine.h"
#include "court.h"

using namespace tinyxml2;

/**
 * @brief Factory for common objects of the program
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
         * Returns the affine transformation stored in configuration file
         * @return affine transformation
         */
        AffineTransformation* createAffineTransformation();

        /**
         * Returns an initialized court according to configuration file
         * @return court
         */
        Court* createCourt();

        /**
         * Returns sequence settings
         * @return sequence settings
         */
        SequenceSettings createSequenceSettings();

        /**
         * Initializes CameraWindow
         */
        void constructCamera();


    private:
        void exploreGraphicsTag();
        void exploreInputTag();
        void exploreOutputTag();
        void exploreAvatarsTag();

        std::vector<float> getSplittenLine(const std::string& line);

        CameraSettings createCameraSettings();
        MoveableSettings createCameraMoveableSettings();
        void fillCameraTrajectory();

        MoveableSettings createGeneralMoveableSettings();

        MovingBody* createBall();
        MoveableSettings createBallMoveableSettings();
        MovingBodySettings createBallBodySettings();

        std::map<int, Player*> createPlayerMap();
        MovingBodySettings createGeneralPlayerBodySettings();
        MoveableSettings createPlayerMoveableSettings();
        PlayerSettings createPlayerSettings();
        std::map<int, Player*> putTrajectories(std::map<int, Player*> playerMap);
        std::map<int, Player*> putJerseyNumber(std::map<int, Player*> playerMap);
        std::map<int, Player*> putModel(std::map<int, Player*> playerMap);

        XMLDocument doc;

        XMLElement* graphicsTag;
        XMLElement* modeTag;
        XMLElement* windowTag;
        XMLElement* guiTextTag;

        XMLElement* inputTag;
        XMLElement* imageTag;
        XMLElement* trackingTag;
        XMLElement* teamsTag;
        XMLElement* transformationTag;

        XMLElement* outputTag;
        XMLElement* videoTag;
        XMLElement* sequenceTag;
        XMLElement* cameraTag;

        XMLElement* avatarsTag;
        XMLElement* sceneTag;
        XMLElement* actionsTag;
        XMLElement* playersTag;
        XMLElement* jerseysTag;
        XMLElement* ballTag;
        XMLElement* colorCurvesTag;
};

#endif // SETTINGSFACTORY_H
