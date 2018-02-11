#include "Model.h"

#include "core/fs/FileWriter.h"
#include "core/fs/FileReader.h"

bool Model::write(const char *path) {
    fs::FileWriter fileWriter(path);
    if (fileWriter.error() != fs::OK) {
        return false;
    }

    ModelWriter modelWriter(fileWriter);
    WriteContext context = { *this, modelWriter };

    write(context);

    return fileWriter.finish() == fs::OK;
}

bool Model::read(const char *path) {
    fs::FileReader fileReader(path);
    if (!fileReader.error() != fs::OK) {
        return false;
    }

    ModelReader modelReader(fileReader);
    ReadContext context = { *this, modelReader };

    read(context);

    return fileReader.finish() == fs::OK;
}

void Model::write(WriteContext &context) const {
    _project.write(context);
    _settings.write(context);
}

void Model::read(ReadContext &context) {
    _project.read(context);
    _settings.read(context);
}
