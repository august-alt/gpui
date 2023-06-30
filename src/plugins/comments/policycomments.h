#ifndef GPUI_POLICY_COMMENTS_H
#define GPUI_POLICY_COMMENTS_H

#include "comment.h"
#include "localizationresourcereference.h"

#include <memory>
#include <vector>


namespace comments
{

/*!
 * \brief The PolicyComments class This outermost (document) element may not be contained by any other elements.
 */
class PolicyComments
{
public:
    /*!
     * \brief revision The revision number.
     */
    uint32_t revision;

    /*!
     * \brief schemaVersion The version number of the applicable schema.
     */
    uint32_t schemaVersion;
    /*!
     * \brief comments  List of comments.
     */
    std::vector<Comment> comments;

    /*!
     * \brief resources Resources.
     */
    std::unique_ptr<LocalizationResourceReference> resources;
};

}

#endif  //_POLICYCOMMENTS_H
