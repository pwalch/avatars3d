/*
  * 3D Avatars
  * Pierre Walch
  */


#ifndef AVATARSFACTORY_H
#define AVATARSFACTORY_H

#include <iostream>
#include "libs/tinyxml2.h"
#include "camerasettings.h"
#include "sequencesettings.h"
#include "affinetransformation.h"
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
    CameraWindow* createCamera();

    /**
     * Creates a camera trajectory input stream
     * @return input stream
     */
    std::istream* createCameraStream();

    /**
     * Creates a player trajectory input stream
     * @return input stream
     */
    std::istream* createPlayerStream();

    /**
     * Creates a ball trajectory input stream
     * @return input stream
     */
    std::istream* createBallStream();

    /**
     * Create camera trajectory chunk obtained from the input stream
     * @param cameraStream stream to expore
     * @param framesToCatch number of frames to get in the stream
     * @return trajectory chunk
     */
    std::pair<VectorSequence, VectorSequence> createCameraChunk(std::istream* cameraStream, int framesToCatch);

    /**
     * Creates a map of chunks, obtained from input stream
     * @param playerStream stream to explore
     * @param playerMap map from player index to new trajectory chunk
     * @param framesToCatch number of frames to get in the stream
     * @return chunk
     */
    std::map<int, VectorSequence > createPlayerChunkMap(std::istream* playerStream,
                                                          std::map<int,Player*> playerMap,
                                                          int framesToCatch);

    /**
     * Create ball trajectory chunk obtained from the input stream
     * @param ballStream stream to expore
     * @param framesToCatch number of frames to get in the stream
     * @return trajectory chunk
     */
    VectorSequence createBallChunk(std::istream* ballStream, int framesToCatch);


private:
    SettingsParser* mSettingsParser;

    MovingBody* createBall();
    std::map<int, Player*> createPlayerMap();

};

#endif // AVATARSFACTORY_H
