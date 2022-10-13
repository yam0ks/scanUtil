#include "WMappedFile.h"

MappedFile::MappedFile(const std::string& filePath)
{
    m_file = CreateFile(filePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if(m_file == INVALID_HANDLE_VALUE)
        return;

    m_fileSize = GetFileSize(m_file, nullptr);

    if(m_fileSize == INVALID_FILE_SIZE)
    {
        CloseHandle(m_file);
        return;
    }

    m_mapping = CreateFileMapping(m_file, nullptr, PAGE_READONLY, 0, 0, nullptr);

    if(m_mapping == nullptr)
    {
        CloseHandle(m_file);
        return;
    }

    m_data = reinterpret_cast<char*>(MapViewOfFile(m_mapping, FILE_MAP_READ, 0, 0, m_fileSize));
}

MappedFile::~MappedFile()
{
    if (m_data != nullptr)
        UnmapViewOfFile(m_data);

    if (m_mapping != nullptr)
        CloseHandle(m_mapping);

    if (m_file != INVALID_HANDLE_VALUE)
        CloseHandle(m_file);
}

bool MappedFile::isOpen() const
{
    return m_data != nullptr;
}

uint64_t MappedFile::size() const
{
    return static_cast<uint64_t>(m_fileSize);
}

const char* MappedFile::data() const
{
    return m_data;
}
