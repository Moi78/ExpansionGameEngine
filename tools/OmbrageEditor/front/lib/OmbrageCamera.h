#ifndef EXPGE_OMBRAGECAMERA_H
#define EXPGE_OMBRAGECAMERA_H

#include <EXP_Game.h>
#include <EXP_Actor.h>
#include <EXP_Camera.h>

namespace Ombrage {
    class OmbrageCamera : public EXP_Actor {
    public:
        OmbrageCamera(EXP_Game *game);

        ~OmbrageCamera();

        void OnStart() override;

        void OnTick() override;

    private:
        std::shared_ptr<EXP_Camera> m_cam_comp;
    };
}


#endif //EXPGE_OMBRAGECAMERA_H
