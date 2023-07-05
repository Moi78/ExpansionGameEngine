//
// Created by mathis on 05/07/23.
//

#ifndef EXPGE_SPIRVOPERATION_H
#define EXPGE_SPIRVOPERATION_H

#include <optional>
#include <cassert>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

#include "SpirvData.h"
#include "SpirvTypes.h"

#include <iostream>

struct SpirvOperation {
    uint32_t op;
    std::vector<uint32_t> words;

    std::optional<uint32_t> result_id;
    std::vector<int32_t> id_repl;

    void LoadOp(uint32_t opid, uint32_t opsize) { op = (opsize << 16) | opid; }

    std::vector<uint32_t> GetData();
    virtual void PreCompile(std::unordered_map<HLTypes, std::shared_ptr<SPVType>> realTypes) {}
};

#endif //EXPGE_SPIRVOPERATION_H
