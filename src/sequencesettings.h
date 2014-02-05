/*
  * 3D Avatars
  * Pierre Walch
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
