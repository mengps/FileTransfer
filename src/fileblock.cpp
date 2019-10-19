#include "fileblock.h"

QDataStream& operator>>(QDataStream &in, FileBlock &block)
{
    in >> block.blockSize
       >> block.offset
       >> block.fileSize
       >> block.fileName
       >> block.dataBlock;

    return in;
}

QDataStream& operator<<(QDataStream &out, FileBlock &block)
{
    out << block.blockSize
        << block.offset
        << block.fileSize
        << block.fileName
        << block.dataBlock;

    return out;
}

QDebug operator<<(QDebug debug, const FileBlock &block)
{
    debug << "[blockSize]: " << block.blockSize << endl
          << "[offset]: " << block.offset << endl
          << "[fileSize]: " << block.fileSize << endl
          << "[fileName]: " << block.fileName << endl;

    return debug;
}
