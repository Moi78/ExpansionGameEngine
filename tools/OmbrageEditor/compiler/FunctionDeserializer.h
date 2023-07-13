#ifndef EXPGE_FUNCTIONDESERIALIZER_H
#define EXPGE_FUNCTIONDESERIALIZER_H

#include <string>
#include <memory>
#include <fstream>

#include "SpirvFunction.h"
#include "SpirvOperation.h"

class FunctionDeserializer {
public:
    FunctionDeserializer() = default;
    ~FunctionDeserializer() = default;

    std::shared_ptr<SpirvFunction> Deserialize(std::string path);
private:
    std::shared_ptr<SpirvOperation> DeserializeOp(std::ifstream& file);
};


#endif //EXPGE_FUNCTIONDESERIALIZER_H
