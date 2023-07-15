#ifndef EXPGE_SHADERNODE_H
#define EXPGE_SHADERNODE_H

#include "imnodes.h"
#include "imgui.h"

#include "Node.h"

#include "SpirvTypes.h"
#include "SpirvConstant.h"

class ShaderNode : public Node {
public:
    ShaderNode(uint32_t id);

    std::string GetNodeFunctionName() override { return "shadernode"; };
    std::vector<HLTypes> GetShaderLayout() {
        return {
            HLTypes::VECTOR4PTRO,
            HLTypes::VECTOR4PTRO,
            HLTypes::VECTOR4PTRO,
            HLTypes::VECTOR4PTRO,
            HLTypes::VECTOR4PTRO
        };
    };

    std::vector<std::shared_ptr<SpirvOperation>> GetStoreOpForNode(std::shared_ptr<SpirvCompiler> compiler,
                                                                   int idx,
                                                                   std::shared_ptr<SpirvDataWrapperBase> toStore);

    std::shared_ptr<SpirvDataWrapperBase> GetDefault(std::shared_ptr<SpirvCompiler> compiler, int idx);
private:
    std::vector<std::shared_ptr<SpirvOperation>> StoreVec3ToVec4(std::shared_ptr<SpirvCompiler> compiler, int layoutidx,
                                                                 std::shared_ptr<SpirvDataWrapperBase> toStore);

    std::vector<std::shared_ptr<SpirvOperation>> StoreFloatToVec4(std::shared_ptr<SpirvCompiler> compiler,
                                                                  int layoutidx,
                                                                  int vecidx,
                                                                  std::shared_ptr<SpirvDataWrapperBase> toStore);

    std::vector<std::shared_ptr<SpirvOperation>> StoreVec4ToVec4(std::shared_ptr<SpirvCompiler> compiler,
                                                                 int layoutidx,
                                                                 std::shared_ptr<SpirvDataWrapperBase> toStore);

    std::shared_ptr<SpirvDataWrapperBase> GetVec40(std::shared_ptr<SpirvCompiler> compiler);
    std::shared_ptr<SpirvDataWrapperBase> GetVec30(std::shared_ptr<SpirvCompiler> compiler);
    std::shared_ptr<SpirvDataWrapperBase> GetFloat0(std::shared_ptr<SpirvCompiler> compiler);
};


#endif //EXPGE_SHADERNODE_H
