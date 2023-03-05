#ifndef GENERICWRITER_H
#define GENERICWRITER_H

#include "io.h"

#include <memory>
#include <string>

namespace io {
    class GPUI_IO_EXPORT GenericWriter
    {
    public:
        virtual ~GenericWriter() = default;

        template<typename TData , typename TFormat>
        bool save(const std::string &filename, TData *fileData);

        template<typename TData, typename TFormat>
        bool save(std::ostream &file, const std::string& plName, TData *fileData);
    };
}

#include "genericwriter.inl"

#endif // GENERICWRITER_H
