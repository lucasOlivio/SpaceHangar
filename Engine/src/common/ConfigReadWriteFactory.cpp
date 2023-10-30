#include "common/ConfigReadWriteFactory.h"
#include "common/ConfigReadWriteJSON.h"
#include "common/utils.h"

iConfigReadWrite* ConfigReadWriteFactory::CreateConfigReadWrite(std::string fileExt)
{
    iConfigReadWrite* pConfigReadWrite = nullptr;
    bool fileLoaded = true;
    // in case the path is been passed
    fileExt = myutils::GetFileExtension(fileExt);

    if (fileExt == "json")
    {
        pConfigReadWrite = new ConfigReadWriteJSON();
    }

    if (!fileLoaded)
    {
        return nullptr;
    }

    return pConfigReadWrite;
}
