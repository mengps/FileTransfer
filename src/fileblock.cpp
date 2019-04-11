#include "fileblock.h"

extern QDataStream& operator>>(QDataStream &in, FileBlock &block)
{
    in >> block.blockSize
       >> block.offset
       >> block.fileSize
       >> block.fileName
       >> block.dataBlock;

    return in;
}

extern QDataStream& operator<<(QDataStream &out, FileBlock &block)
{
    out << block.blockSize
        << block.offset
        << block.fileSize
        << block.fileName
        << block.dataBlock;

    return out;
}

extern QDebug operator<<(QDebug debug, const FileBlock &block)
{
    debug << "[blockSize]: " << block.blockSize << endl
          << "[offset]: " << block.offset << endl
          << "[fileSize]: " << block.fileSize << endl
          << "[fileName]: " << block.fileName << endl;

    return debug;
}
