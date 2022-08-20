#ifndef EXPGE_RD_RENDERINGPIPELINE_H
#define EXPGE_RD_RENDERINGPIPELINE_H

#include "RD_API.h"
#include "RD_RenderPass.h"
#include "RD_Pipeline.h"
#include "RD_Texture.h"
#include "RD_ShaderLoader.h"

#include "RD_Camera.h"

#include <memory>
#include <vector>

class RD_RenderingPipeline {
public:
    RD_RenderingPipeline() {};
    virtual ~RD_RenderingPipeline() {};

    virtual bool InitRenderingPipeline() = 0;

    virtual void RenderScene(std::vector<std::shared_ptr<RD_IndexedVertexBuffer>>& sceneData) = 0;
};

class RD_RenderingPipeline_PBR : public RD_RenderingPipeline {
public:
    RD_RenderingPipeline_PBR(std::shared_ptr<RD_API>& api);
    ~RD_RenderingPipeline_PBR() override;

    bool InitRenderingPipeline() override;

    void RenderScene(std::vector<std::shared_ptr<RD_IndexedVertexBuffer>>& sceneData) override;

private:
    std::shared_ptr<RD_API> m_api;

    std::shared_ptr<RD_RenderPass> m_rpass;
    std::shared_ptr<RD_Pipeline> m_plineGBuff;

    std::unique_ptr<RD_Camera> m_cam;
};


#endif //EXPGE_RD_RENDERINGPIPELINE_H
