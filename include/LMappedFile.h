#pragma once

#include <string>

class MappedFile
{
public:
    explicit MappedFile(const std::string& path);
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

    int m_descriptor = -1;
    uint64_t m_size = -1;
};
