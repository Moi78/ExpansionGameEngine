#ifndef EXPGE_FUNCTIONSERIALIZER_H
#define EXPGE_FUNCTIONSERIALIZER_H

#include "SpirvFunction.h"
#include "SpirvOperation.h"

#include <fstream>
#include <memory>

class FunctionSerializer {
public:
    FunctionSerializer() = default;
    ~FunctionSerializer() = default;

    bool Serialize(std::shared_ptr<SpirvFunction> func, std::string outpath);

private:
    std::vector<char> SerializeOp(std::shared_ptr<SpirvOperation> op);
};


#endif //EXPGE_FUNCTIONSERIALIZER_H
