#include "EXP_BoxBody.h"

EXP_BoxBody::EXP_BoxBody(std::shared_ptr<EXP_PhysicsHandler> handler, vec3 pos, vec3 scale, vec3 rot, bool isStatic) :
EXP_Body(handler, pos, rot, scale, isStatic)
{
    JPH::BodyInterface& bodyInterface = handler->GetBodyInterface();
    JPH::BoxShapeSettings shapeSettings(JPH::Vec3(scale.GetX(), scale.GetY(), scale.GetZ()));

    JPH::ShapeSettings::ShapeResult shape = shapeSettings.Create();
    JPH::ShapeRefC createdShape = shape.Get();

    CreateBodyFromShape(bodyInterface, createdShape, pos, rot, scale);
}
