#include <falcon/engine.h>
#include "common.h"
#include "GameEngine.h"
#include "PhysicsSystem.h"
#include "AppFalcon.h"


FALCON_FUNC fal_Physics_SetGravity(Falcon::VMachine *vm)
{
    FALCON_REQUIRE_PARAMS_EXTRA(1, "N");
    GameEngine::GetInstance()->physmgr->envPhys.gravity = float(vm->param(0)->forceNumeric());
}

FALCON_FUNC fal_Physics_GetGravity(Falcon::VMachine *vm)
{
    vm->retval(GameEngine::GetInstance()->physmgr->envPhys.gravity);
}


Falcon::Module *FalconGameModule_create(void)
{
    Falcon::Module *m = new Falcon::Module;
    m->name("GameModule");

    Falcon::Symbol *symPhysics = m->addSingleton("Physics");
    Falcon::Symbol *clsPhysics = symPhysics->getInstance();
    m->addClassMethod(clsPhysics, "SetGravity", fal_Physics_SetGravity);
    m->addClassMethod(clsPhysics, "GetGravity", fal_Physics_GetGravity);

    return m;
};


