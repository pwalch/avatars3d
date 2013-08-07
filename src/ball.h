#ifndef BALL_H
#define BALL_H

#include <map>
#include <irrlicht.h>

class Ball
{
    public:
        Ball();
        void mapTime(int time, irr::core::vector3df position);
        void init(const char* modelPath, const char* texturePath);
        void setTime(int time);

        void smoothTrajectory(int frameNumber);
    private:
        irr::scene::ISceneNode* node;
        irr::video::ITexture* texture;

        std::map<int, irr::core::vector3df> trajectory;
};

#endif // BALL_H
