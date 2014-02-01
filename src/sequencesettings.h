/*
  * 3D Avatars
  * Pierre Walch
  */


#ifndef SEQUENCESETTINGS_H
#define SEQUENCESETTINGS_H

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
            mFrameNumber = 0;
            mFramerate = 0;
            mStartTime = 0;
            mEndTime = 0;
            mCurrentTime = 0;
            mVideoOutputName = "";
            mSpeedInterval = 0;
            mNbPointsAverager = 0;
        }

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
         * Instant of time currently displayed
         */
        int mCurrentTime;

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
