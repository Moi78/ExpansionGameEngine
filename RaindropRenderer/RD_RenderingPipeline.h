#ifndef EXPGE_RD_RENDERINGPIPELINE_H
#define EXPGE_RD_RENDERINGPIPELINE_H

#include "RD_API.h"
#include "RD_RenderPass.h"
#include "RD_Pipeline.h"
#include "RD_Texture.h"
#include "RD_ShaderLoader.h"
#include "RD_UniformBuffer.h"
#include "RD_Quad.h"

#include "RD_Mesh.h"
#include "RD_Camera.h"
#include "RD_DirLight.h"

#include <memory>
#include <vector>

class RD_RenderingPipeline {
public:
    RD_RenderingPipeline() {};
    virtual ~RD_RenderingPipeline() {};

    virtual bool InitRenderingPipeline() = 0;
    virtual void Resize(int w, int h) = 0;

    virtual void RenderScene(std::vector<std::shared_ptr<RD_Mesh>>& sceneData) = 0;
};

class RD_RenderingPipeline_PBR : public RD_RenderingPipeline {
public:
    RD_RenderingPipeline_PBR(std::shared_ptr<RD_API>& api);
    ~RD_RenderingPipeline_PBR() override;

    bool InitRenderingPipeline() override;
    void Resize(int w, int h) override;

    void RenderScene(std::vector<std::shared_ptr<RD_Mesh>>& sceneData) override;

private:
    std::shared_ptr<RD_API> m_api;
    std::shared_ptr<RD_Quad> m_renderSurface;

    std::shared_ptr<RD_RenderPass> m_rpassGBuff;
    std::shared_ptr<RD_Pipeline> m_plineGBuff;

    std::shared_ptr<RD_RenderPass> m_rpassLight;
    std::shared_ptr<RD_Pipeline> m_plineLight;

    std::shared_ptr<RD_UniformBuffer> m_camModel;
    std::shared_ptr<RD_UniformBuffer> m_dlights;

    std::unique_ptr<RD_Camera> m_cam;
};


#endif //EXPGE_RD_RENDERINGPIPELINE_H
