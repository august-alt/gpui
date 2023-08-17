find . \( -name "*.cpp" -or -name "*.h" -or -name ".hpp" -or -name "*.in" -or -name "*.inl" \) -exec sed -i 's/qDebug()/GPUI_DEBUG_STREAM/g' {} \;
find . \( -name "*.cpp" -or -name "*.h" -or -name ".hpp" -or -name "*.in" -or -name "*.inl" \) -exec sed -i 's/qInfo()/GPUI_INFO_STREAM/g' {} \;
find . \( -name "*.cpp" -or -name "*.h" -or -name ".hpp" -or -name "*.in" -or -name "*.inl" \) -exec sed -i 's/qWarning()/GPUI_WARNING_STREAM/g' {} \;

for file in $(find . -name "*.cpp" -or -name "*.h" -or -name ".hpp" -or -name "*.in" -or -name "*.inl")
do
    file_dir=$(dirname "$file")
    include_path=$(realpath --relative-to="$file_dir" "/home/SMB.BASEALT.RU/cherniginma/Source/wip/gpui/src/core/logger/log.h")
    sed -i "s@#include <QDebug>@#include \"$include_path\"@" "$file"
done
