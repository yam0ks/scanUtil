#pragma once

#include <string>
#include <windows.h>

class MappedFile
{
public:
    MappedFile(const std::string& filePath);
    ~MappedFile();

    MappedFile(const MappedFile&) = delete;
    MappedFile& operator=(const MappedFile&) = delete;

    MappedFile(MappedFile&&) = default;
    MappedFile& operator=(MappedFile&&) = default;

    bool isOpen() const;

    uint64_t size() const;
    const char* data() const;

private:
    char* m_data = nullptr;

    HANDLE m_file = INVALID_HANDLE_VALUE;
    DWORD m_fileSize = INVALID_FILE_SIZE;
    HANDLE m_mapping = nullptr;
};
