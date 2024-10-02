#include <cmath>
#include "GameObject.h"
#include "Static.h"

SDL_Texture* GameObject::getCurrentTexture(void)
{
    return this->textureList[this->frame];
}

void GameObject::updatePosition(Struct::Coordinate coord)
{
    this->leftBelowPoint = coord;
}

void GameObject::moveBy(Struct::Coordinate coord)
{
    this->leftBelowPoint.x += coord.x;
    this->leftBelowPoint.y += coord.y;
}

void GameObject::rotate(Struct::RotateDir direction)
{
    if (direction == Struct::ROTATE_CW) {
        switch (this->direction) {
        case Struct::DIR_SOUTH :
            this->direction = Struct::DIR_EAST;
            break;
        case Struct::DIR_EAST:
            this->direction = Struct::DIR_NORTH;
            break;
        case Struct::DIR_NORTH:
            this->direction = Struct::DIR_WEST;
            break;
        case Struct::DIR_WEST:
            this->direction = Struct::DIR_SOUTH;
            break;
        }
        return;
    }
    switch (this->direction) {
    case Struct::DIR_SOUTH:
        this->direction = Struct::DIR_WEST;
        break;
    case Struct::DIR_EAST:
        this->direction = Struct::DIR_SOUTH;
        break;
    case Struct::DIR_NORTH:
        this->direction = Struct::DIR_EAST;
        break;
    case Struct::DIR_WEST:
        this->direction = Struct::DIR_NORTH;
        break;
    }
}

Struct::Coordinate GameObject::getPosition(void)
{
    return this->leftBelowPoint;
}

int GameObject::getWidth(void)
{
    return this->width;
}

int GameObject::getHeight(void)
{
    return this->height;
}

Struct::Direction GameObject::getDirection(void)
{
    return this->direction;
}

void GameObject::gotoNextFrame(int num)
{
    this->frame = (this->frame + num) % this->frameNumber;
}

void SequentialGameObject::setFrameSpeed(double frameSpeed)
{
    this->frameSpeed = frameSpeed;
}

void SequentialGameObject::updateFrame(long long currentClock)
{
    int jumpFrames = floor((currentClock - this->preRenderPoint) / (1000 * abs(this->frameSpeed)));
    if (jumpFrames > 0) {
        if (this->frameSpeed >= 0) this->gotoNextFrame(jumpFrames);
        else this->gotoNextFrame((-1) * jumpFrames);
        this->preRenderPoint = currentClock;
    }
}
