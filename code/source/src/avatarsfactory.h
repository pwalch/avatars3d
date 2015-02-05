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
    explicit AvatarsFactory(std::string cfgPath);

    /**
     * Releases memory of the SettingsParser instance
     */
    virtual ~AvatarsFactory();

    /**
     * Returns an initialized court according to configuration file
     * @return initialized court
     */
    std::unique_ptr<Court> createCourt() const;

    /**
     * Returns sequence settings
     * @return sequence settings
     */
    SequenceSettings retrieveSequenceSettings() const;

    /**
     * Creates an affine transformation by retrieving its settings from configuration file
     * @return pointer to affine transformation
     */
    std::unique_ptr<AffineTransformation> createTransformation() const;

    /**
     * Creates camera window instance and initializes it
     */
    std::unique_ptr<CameraWindow> createCamera() const;

    /**
     * Creates a camera trajectory input stream
     * @return input stream
     */
    std::unique_ptr<std::istream> createCameraStream() const;

    /**
     * Creates a player trajectory input stream
     * @return input stream
     */
    std::unique_ptr<std::istream> createPlayerStream() const;

    /**
     * Creates a ball trajectory input stream
     * @return input stream
     */
    std::unique_ptr<std::istream> createBallStream() const;

    /**
     * Create camera trajectory chunk obtained from the input stream
     * @param cameraStream stream to expore
     * @param framesToCatch number of frames to get in the stream
     * @return trajectory chunk
     */
    const std::pair<VectorSequence, VectorSequence> createCameraChunk(std::istream& cameraStream, int framesToCatch) const;

    /**
     * Creates a map of chunks, obtained from input stream
     * @param playerStream stream to explore
     * @param playerMap map of players
     * @param framesToCatch number of frames to get in the stream
     * @return chunk
     */
    const std::map<int, VectorSequence > createPlayerChunkMap(std::istream& playerStream,
                                                          const std::map<int, std::unique_ptr<Player> >& playerMap,
                                                          int framesToCatch) const;

    /**
     * Create ball trajectory chunk obtained from the input stream
     * @param ballStream stream to expore
     * @param framesToCatch number of frames to get in the stream
     * @return trajectory chunk
     */
    const VectorSequence createBallChunk(std::istream& ballStream, int framesToCatch) const;


private:
    std::unique_ptr<SettingsParser> mSettingsParser;

    std::unique_ptr<MovingBody> createBall() const;
    std::unique_ptr<PlayerMap> createPlayerMap() const;

};

#endif // AVATARSFACTORY_H
