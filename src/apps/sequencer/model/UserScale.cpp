#include "UserScale.h"

UserScale::Array UserScale::userScales;

UserScale::UserScale() :
    Scale("")
{
    clear();
}

void UserScale::clear() {
    StringUtils::copy(_name, "INIT", sizeof(_name));
    setMode(Mode::Note);
    clearItems();
}

void UserScale::clearItems() {
    setSize(1);
    _items.fill(0);
    if (_mode == Mode::Voltage) {
        _items[1] = 1000;
    }
}

void UserScale::write(WriteContext &context) const {
    auto &writer = context.writer;
    writer.write(_mode);
    writer.write(_size);
    for (int i = 0; i < _size; ++i) {
        writer.write(_items[i]);
    }
}

void UserScale::read(ReadContext &context) {
    clear();

    auto &reader = context.reader;
    reader.read(_mode);
    reader.read(_size);
    for (int i = 0; i < _size; ++i) {
        reader.read(_items[i]);
    }
}

fs::Error UserScale::write(const char *path) const {
    fs::FileWriter fileWriter(path);
    if (fileWriter.error() != fs::OK) {
        fileWriter.error();
    }

    FileHeader header(FileType::UserScale, 0, _name);
    fileWriter.write(&header, sizeof(header));

    Writer writer(fileWriter, Version);
    WriteContext context = { writer };
    write(context);

    return fileWriter.finish();
}

fs::Error UserScale::read(const char *path) {
    fs::FileReader fileReader(path);
    if (fileReader.error() != fs::OK) {
        fileReader.error();
    }

    FileHeader header;
    fileReader.read(&header, sizeof(header));
    header.readName(_name, sizeof(_name));

    Reader reader(fileReader, Version);
    ReadContext context = { reader };
    read(context);

    return fileReader.finish();
}
