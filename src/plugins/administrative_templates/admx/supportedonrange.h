#include <memory>
#include <string>

namespace model
{
namespace admx
{
/*!
 * \brief The SupportedOnRange class The supportedOnRange element provides supported version range.
 */
class SupportedOnRange
{
public:
    /*!
     * \brief A reference to a supported product definition, either a single version or a complex definition.
     */
    std::string itemReference{};

    /*!
     * \brief Minimum supported version.
     */
    uint32_t minVersionIndex{};

    /*!
     * \brief Maximum supported version.
     */
    uint32_t maxVersionIndex{};

    SupportedOnRange(const std::string &reference, uint32_t minVersion, uint32_t maxVersion)
        : itemReference(reference)
        , minVersionIndex(minVersion)
        , maxVersionIndex(maxVersion)
    {}
};

} // namespace admx
} // namespace model
