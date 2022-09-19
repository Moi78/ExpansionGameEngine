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
#include "RD_PointLight.h"

#include <memory>
#include <vector>

struct RD_CasterCount {
    int nDLights;
    int nPlights;
};

class RD_RenderingPipeline {
public:
    RD_RenderingPipeline() {};
    virtual ~RD_RenderingPipeline() {};

    virtual bool InitRenderingPipeline(std::string enginePath) = 0;
    virtual void Resize(int w, int h) = 0;

    virtual void RenderScene(std::vector<std::shared_ptr<RD_Mesh>>& sceneData, std::shared_ptr<RD_Camera> cam) = 0;

    virtual void PushDirLight(std::shared_ptr<RD_DirLight> dlight, int index) = 0;
    virtual void PushPointLight(std::shared_ptr<RD_PointLight> plight, int index) = 0;
    virtual void PushCasterCount(RD_CasterCount& ccount) = 0;

    virtual std::shared_ptr<RD_RenderPass> GetBaseRenderpass() = 0;
};

class RD_RenderingPipeline_PBR : public RD_RenderingPipeline {
public:
    RD_RenderingPipeline_PBR(std::shared_ptr<RD_API>& api);
    ~RD_RenderingPipeline_PBR() override;

    bool InitRenderingPipeline(std::string enginePath) override;
    void Resize(int w, int h) override;

    void RenderScene(std::vector<std::shared_ptr<RD_Mesh>>& sceneData, std::shared_ptr<RD_Camera> cam) override;

    void PushDirLight(std::shared_ptr<RD_DirLight> dlight, int index) override;
    void PushPointLight(std::shared_ptr<RD_PointLight> plight, int index) override;
    void PushCasterCount(RD_CasterCount& ccount) override;

    std::shared_ptr<RD_RenderPass> GetBaseRenderpass() override;

private:
    std::shared_ptr<RD_API> m_api;
    std::shared_ptr<RD_Quad> m_renderSurface;

    std::shared_ptr<RD_RenderPass> m_rpassGBuff;
    std::shared_ptr<RD_Pipeline> m_plineGBuff;

    std::shared_ptr<RD_RenderPass> m_rpassLight;
    std::shared_ptr<RD_Pipeline> m_plineLight;

    std::shared_ptr<RD_UniformBuffer> m_camModel;

    std::shared_ptr<RD_UniformBuffer> m_dlights;
    std::shared_ptr<RD_UniformBuffer> m_plights;
    std::shared_ptr<RD_UniformBuffer> m_casterCount;
};


#endif //EXPGE_RD_RENDERINGPIPELINE_H
