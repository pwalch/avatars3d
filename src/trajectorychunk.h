#ifndef TRAJECTORYCHUNK_H
#define TRAJECTORYCHUNK_H

#include <map>
#include "irrlicht.h"

using namespace irr::core;

/**
 * Represents a small piece of trajectory, which is then appended to a TrajectoryData object
 */
class TrajectoryChunk
{
public:
    /**
     * Constructor
     * @param positions virtual position map
     * @param rotations virtual rotation map
     */
    TrajectoryChunk(const std::map<int, vector3df>& positions,
                    const std::map<int, vector3df>& rotations);

    /**
     * Returns position map
     * @return positions
     */
    const std::map<int, vector3df>& getPositions() const;

    /**
     * Returns rotation map
     * @return rotation
     */
    const std::map<int, vector3df>& getRotations() const;

    /**
     * Returns first frame index of the chunk
     * @return first frame index
     */
    int getBeginIndex();

private:
    std::map<int, vector3df> mPositions;
    std::map<int, vector3df> mRotations;
};

#endif // TRAJECTORYCHUNK_H
