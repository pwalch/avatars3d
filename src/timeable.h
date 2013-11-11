#ifndef TIMEABLE_H
#define TIMEABLE_H

/**
 * @brief Represents an object having different states over time
 *
 */
class Timeable
{
    public:
        /**
         * Abstract method to adapt object state to the wanted time value
         * @param time wanted frame time
         */
        virtual void setTime(int time) = 0;
};

#endif // TIMEABLE_H
