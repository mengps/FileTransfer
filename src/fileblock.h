#ifndef FILEBLOCK_H
#define FILEBLOCK_H

#include <QDebug>
#include <QtGlobal>
#include <QDataStream>

struct FileBlock
{
    qint16 blockSize;
    qint32 offset;
    qint32 fileSize;
    QByteArray fileName;
    QByteArray dataBlock;

    bool isEmpty() const {
        return fileName.isEmpty() || dataBlock.isEmpty();
    }

    int size() const {
        return int(sizeof(blockSize)) +
               int(sizeof(offset)) +
               int(sizeof(fileSize)) +
               fileName.size() +
               dataBlock.size() +
               2 * int(sizeof(int));       //有两个QByteArray，每个会在前面加int大小
    }
};

extern QDataStream& operator>>(QDataStream &in, FileBlock &block);
extern QDataStream& operator<<(QDataStream &out, FileBlock &block);
extern QDebug operator<<(QDebug debug, const FileBlock &block);

#endif // FILEBLOCK_H
