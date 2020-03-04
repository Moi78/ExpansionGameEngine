#pragma once

#ifndef _EXP_RIGID_BODY_H__
#define _EXP_RIGID_BODY_H__

#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
#define EXPGE_API __declspec(dllexport)
#else
#define EXPGE_API __declspec(dllimport)
#endif

#include <iostream>

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

class EXPGE_API EXP_RigidBody
{
};

#endif //_EXP_RIGID_BODY_H__