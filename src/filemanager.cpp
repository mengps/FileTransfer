#include "filemanager.h"
#include <QDebug>

FileInfo::FileInfo(QObject *parent)
    : QObject (parent)
{

}

FileInfo::FileInfo(const FileInfo &other, QObject *parent)
    : QObject (parent),
      m_fileName(other.m_fileName),
      m_offset(other.m_offset),
      m_fileSize(other.m_fileSize)
{

}

FileInfo::FileInfo(const QString &fileName, int offset, int fileSize, QObject *parent)
    : QObject (parent),
      m_fileName(fileName),
      m_offset(offset),
      m_fileSize(fileSize)
{

}

FileInfo::~FileInfo()
{

}

QString FileInfo::fileName() const
{
    return m_fileName;
}

void FileInfo::setFileName(const QString &fileName)
{
    if (fileName != m_fileName) {
        m_fileName = fileName;
        emit fileNameChanged();
    }
}

int FileInfo::offset() const
{
    return m_offset;
}

void FileInfo::setOffset(int offset)
{
    if (offset != m_offset) {
        m_offset = offset;
        emit offsetChanged();
    }
}

int FileInfo::fileSize() const
{
    return m_fileSize;
}

void FileInfo::setFileSize(int fileSize)
{
    if (fileSize != m_fileSize) {
        m_fileSize = fileSize;
        emit fileSizeChanged();
    }
}


FileManager* FileManager::instance()
{
    static FileManager fileManager;
    return &fileManager;
}

FileManager::FileManager(QObject *parent)
    : QObject (parent)
{
    m_readFilesProxy = new QQmlListProperty<FileInfo>(this, m_readFiles);
    m_writeFilesProxy = new QQmlListProperty<FileInfo>(this, m_writeFiles);
}

FileManager::~FileManager()
{

}

QQmlListProperty<FileInfo> FileManager::readFiles()
{
    return *m_readFilesProxy;
}

QQmlListProperty<FileInfo> FileManager::writeFiles()
{
    return *m_writeFilesProxy;
}

void FileManager::addReadFile(const QString &fileName, qint32 totalSize)
{
    FileInfo *info = new FileInfo(fileName, 0, totalSize, this);
    m_readFiles.append(info);
    m_filesTable[fileName] = info;
    emit readFilesChanged();
}

void FileManager::updateReadFile(const QString &fileName, qint32 offset)
{
    FileInfo *info = m_filesTable[fileName];
    if (info) {
        info->setOffset(offset);
        if (offset == info->fileSize()) {
            m_filesTable.remove(fileName);
            emit writeFileComplete(fileName);
        }
    }
}

void FileManager::addWriteFile(const QString &fileName, qint32 totalSize)
{
    FileInfo *info = new FileInfo(fileName, 0, totalSize, this);
    m_writeFiles.append(info);
    m_filesTable[fileName] = info;
    emit writeFilesChanged();
}

void FileManager::updateWriteFile(const QString &fileName, qint32 offset)
{
    FileInfo *info = m_filesTable[fileName];
    if (info) {
        info->setOffset(offset);
        if (offset == info->fileSize()) {
            m_filesTable.remove(fileName);
            emit writeFileComplete(fileName);
        }
    }
}
