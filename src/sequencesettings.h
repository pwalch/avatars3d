#ifndef SEQUENCESETTINGS_H
#define SEQUENCESETTINGS_H

/**
 * @brief Sequence settings
 * Contains all the properties related to sequences
 */
class SequenceSettings
{
    public:

        SequenceSettings() {
            mFrameNumber = 0;
            mFramerate = 0;
            mStartTime = 0;
            mEndTime = 0;
            mCurrentTime = 0;
            mName = "";
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
        std::string mName;

};

#endif // SEQUENCESETTINGS_H
