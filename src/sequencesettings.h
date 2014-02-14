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

#ifndef SEQUENCESETTINGS_H
#define SEQUENCESETTINGS_H


enum RUN_MODE { MODE_GUI = 0, MODE_CONSOLE = 1, MODE_LIVE = 2 };

/**
 * @brief Sequence settings
 *
 * Contains all the properties related to sequences
 */
class SequenceSettings
{
public:

    /**
     * Creates an empty object with default values
     */
    SequenceSettings() {
        mMode = MODE_GUI;
        mFrameNumber = 0;
        mFramerate = 0;
        mStartTime = 0;
        mEndTime = 0;
        mInitialTime = 0;
        mVideoOutputName = "";
        mSpeedInterval = 0;
        mNbPointsAverager = 0;
    }

    /**
     * Execution mode of the program
     */
    RUN_MODE mMode;

    /**
     * Number of frames in the sequence
     */
    int mFrameNumber;

    /**
     * Frame rate of the sequence
     */
    int mFramerate;

    /**
     * Start of the sub-part of the sequence to record
     */
    int mStartTime;

    /**
     * End of the sub-part of the sequence to record
     */
    int mEndTime;

    /**
     * Instant of time displayed just after initialization
     */
    int mInitialTime;

    /**
     * Name of output file of the sub-sequence to record
     */
    std::string mVideoOutputName;

    /**
     * Interval for speed computation (derivative)
     */
    int mSpeedInterval;

    /**
     * Number of points for N-points averager
     */
    int mNbPointsAverager;

};

#endif // SEQUENCESETTINGS_H
