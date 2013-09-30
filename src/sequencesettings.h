#ifndef SEQUENCESETTINGS_H
#define SEQUENCESETTINGS_H

/**
 * @brief Sequence settings
 * Contains all the properties related to sequences
 */
class SequenceSettings
{
    public:

        /**
         * Number of frames in the sequence
         */
        int frameNumber;

        /**
         * Frame rate of the sequence
         */
        int framerate;

        /**
         * Start of the sub-part of the sequence to record
         */
        int startTime;

        /**
         * End of the sub-part of the sequence to record
         */
        int endTime;

        /**
         * Instant of time currently displayed
         */
        int currentTime;

};

#endif // SEQUENCESETTINGS_H
