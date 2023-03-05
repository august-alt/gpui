#ifndef SCRIPTSTREEMODEL_H
#define SCRIPTSTREEMODEL_H

#include <mvvm/model/sessionmodel.h>

#include "../../../src/core/common.h"

namespace scripts_plugin
{
class GPUI_SYMBOL_EXPORT ScriptsTreeModel : public ModelView::SessionModel
{
public:
    explicit ScriptsTreeModel();

private:
    void populateModel();
};
} // namespace scripts_plugin
#endif // SCRIPTSTREEMODEL_H
