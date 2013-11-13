/**
  * 3D Avatars
  * Pierre Walch
  */


#ifndef TIMEABLE_H
#define TIMEABLE_H

/**
 * @brief Interface for objects having a different state over time
 *
 * Provides an interface for all the objects depending on time in the program.
 */
class Timeable
{
    public:
        /**
         * Abstract method to adapt object state to the wanted time value
         * @param time frame index
         */
        virtual void setTime(int time) = 0;
};

#endif // TIMEABLE_H
