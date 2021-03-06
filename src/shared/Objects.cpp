#include <stack>

#include "common.h"
#include "SharedDefines.h"
#include "Objects.h"
#include "LayerMgr.h"

#include "UndefUselessCrap.h"

BaseObject::BaseObject()
{
    _falObj = NULL;
    _layermgr = NULL;
    _id = 0;
}

BaseObject::~BaseObject()
{
    DEBUG(logdebug("~BaseObject "PTRFMT, this));
}

// single-directional non-recursive DFS graph search
void BaseObject::GetAttached(std::set<BaseObject*>& found) const
{
    std::stack<const BaseObject*> pending;
    pending.push(this);

    const BaseObject *obj;
    do 
    {
        obj = pending.top();
        pending.pop();
        found.insert(const_cast<BaseObject*>(obj));

        for(std::set<BaseObject*>::const_iterator it = obj->_children.begin(); it != obj->_children.end(); ++it)
        {
            if(found.find(*it) == found.end())
                pending.push(*it);
        }
    }
    while (pending.size());
}

void ActiveRect::Init(void)
{
    type = OBJTYPE_RECT;
    _collisionEnabled = true;
    _update = false;
}

void ActiveRect::SetBBox(float x_, float y_, uint32 w_, uint32 h_)
{
    BaseRect::SetBBox(x_, y_, w_, h_);
    HasMoved();
}

void ActiveRect::SetPos(float x_, float y_)
{
    BaseRect::SetPos(x_, y_);
    HasMoved();
}

void ActiveRect::Move(float xr, float yr)
{
    SetPos(x + xr, y + yr);
    HasMoved();
}

// returns our side on which 'other' collided with us
// (side of 'other')
// TODO: optimize this function! it should be possible to do this a lot simpler...
uint8 BaseRect::CollisionWith(BaseRect *other)
{
    int32 ix = int32(x);
    int32 iy = int32(y);
    int32 oix = int32(other->x);
    int32 oiy = int32(other->y);

    // self lower right corner
    int32 ax1 = ix + w - 1;
    int32 ay1 = iy + h - 1;

    // other lower right corner
    int32 bx1 = oix + other->w - 1;
    int32 by1 = oiy + other->h - 1;

    //check if bounding boxes intersect
    if((bx1 < ix) || (ax1 < oix))
        return SIDE_NONE;
    if((by1 < iy) || (ay1 < oiy))
        return SIDE_NONE;

    int32 xstart = std::max(ix,oix);
    int32 xend = std::min(ax1,bx1);

    int32 ystart = std::max(iy,oiy);
    int32 yend = std::min(ay1,by1);

    int32 width = xend - xstart;
    int32 height = yend - ystart;

    // TODO: if 'other' is completely contained in 'this', this test is always returns SIDE_RIGHT
    //       just noting this here in case this has to be corrected someday, but for now it seems that its not necessary

    // check if 'this' is completely contained in 'other'
    if(ix >= oix && ax1 <= bx1 && iy >= oiy && ay1 <= by1)
    {
        // calculate both centers and guess which side we came from
        int32 xc = ix + (width / 2);
        int32 yc = iy + (height / 2);
        int32 other_xc = oix + (other->w / 2);
        int32 other_yc = oiy + (other->h / 2);
        int32 xcd = abs(xc - other_xc);
        int32 ycd = abs(yc - other_yc);
        if(ycd >= xcd) // height diff is greater than width diff, so we came from top or bottom
        {
            if(yc < other_yc)
                return SIDE_BOTTOM;
            else
                return SIDE_TOP;
        }
        else // width diff is greater, so we came from left or right
        {
            if(xc < other_xc)
                return SIDE_RIGHT;
            else
                return SIDE_LEFT;
        }
    }

    if(height >= width) // must be left or right
    {
        if(xstart == oix)
            return SIDE_RIGHT;
        else
            return SIDE_LEFT;
    }
    else // must be top or bottom
    {
        if(ystart == oiy)
            return SIDE_BOTTOM;
        else
            return SIDE_TOP;
    }

    NOT_REACHED_LINE;
}

void ActiveRect::AlignToSideOf(ActiveRect *other, uint8 side)
{
    DEBUG(ASSERT(side != SIDE_NONE));

    int32 oldix = uint32(this->x);
    int32 oldiy = uint32(this->y);

    if(GetId() >= OBJTYPE_OBJECT)
    {
        Object *self = (Object*)this;
        // stop movement if required
        if(side & (SIDE_TOP | SIDE_BOTTOM))
            self->phys.yspeed = 0.0f;
        if(side & (SIDE_LEFT | SIDE_RIGHT))
            self->phys.xspeed = 0.0f;
    }

    switch(side)
    {
        case SIDE_TOP:
            this->y = other->y - this->h;
            break;

        case SIDE_BOTTOM:
            this->y = other->y + other->h;
            break;

        case SIDE_LEFT:
            this->x = other->x - this->w;

        case SIDE_RIGHT:
            this->x = other->x + other->w;
    }

    if(oldix != int32(this->x) || oldiy != int32(this->y))
        SetMoved(true);
}

uint32 ActiveRect::CanMoveToDirection(uint8 d, uint32 pixels /* = 1 */)
{
    return _layermgr->CanMoveToDirection((BaseRect*)this, d, pixels);
}

float ActiveRect::GetDistanceX(ActiveRect *other) const
{
    float result;
    if(this->x < other->x)
        result = other->x - (this->x + this->w);
    else
        result = this->x - (other->x + other->w);
    return result < 0.0f ? 0.0f : result;
}

float ActiveRect::GetDistanceY(ActiveRect *other) const
{
    float result;
    if(this->y < other->y)
        result = other->y - (this->y + this->h);
    else
        result = this->y - (other->y + other->h);
    return result < 0.0f ? 0.0f : result;
}

float ActiveRect::GetDistance(ActiveRect *other) const
{
    float x = GetDistanceX(other);
    float y = GetDistanceY(other);
    return sqrt(x*x + y*y);
}


void Object::Init(void)
{
    type = OBJTYPE_OBJECT;
    _GenericInit();
}

Object::~Object(void)
{
    SetSprite(NULL); // this will handle refcounting
}

void Object::_GenericInit(void)
{
    memset(&phys, 0, sizeof(PhysProps)); // TODO: apply some useful default values
    _physicsAffected = false;
    _oldLayerId = _layerId = LAYER_MAX / 2; // place on middle layer by default
    _gfx = NULL;
    _moved = true; // do collision detection on spawn
    _collisionEnabled = true; // do really do collision detetion
    gfxoffsx = gfxoffsy = 0;
    _oldLayerRect.x = 0;
    _oldLayerRect.y = 0;
    _oldLayerRect.w = 0;
    _oldLayerRect.h = 0;
    _blocking = false;
    _update = true;
    _visible = true;
}

void Object::SetSprite(BasicTile *tile)
{
    if(tile == _gfx)
        return;
    if(tile)
        tile->ref++;
    if(_gfx)
        _gfx->ref--;
    _gfx = tile;
}

void Unit::Init(void)
{
    type = OBJTYPE_UNIT;
    _GenericInit();
}

void Player::Init(void)
{
    type = OBJTYPE_PLAYER;
    _GenericInit();
}
