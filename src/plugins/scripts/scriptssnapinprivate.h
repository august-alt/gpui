#ifndef SCRIPTSSNAPINPRIVATE_H
#define SCRIPTSSNAPINPRIVATE_H

#include "scriptsmodel.h"
#include "scriptsmodelio.h"
#include "scriptssnapin.h"
#include "scriptstreemodel.h"
#include "scriptstreeproxymodel.h"

#include "../../gui/mainwindow.h"

#include <QObject>
#include <QTranslator>

#include <mvvm/factories/viewmodelfactory.h>
#include <mvvm/viewmodel/viewmodel.h>

namespace scripts_plugin
{
class ScriptsSnapIn;
class ScriptsTreeProxyModel;

class ScriptsSnapInPrivate : public QObject
{
public:
    Q_OBJECT

public:
    std::string policyPath                = "";
    gpui::MainWindow *mainWindow          = nullptr;

    std::unique_ptr<ScriptsModel> userScriptsModel         = std::make_unique<ScriptsModel>();
    std::unique_ptr<ScriptsModel> userPowerScriptsModel    = std::make_unique<ScriptsModel>();
    std::unique_ptr<ScriptsModel> machineScriptsModel      = std::make_unique<ScriptsModel>();
    std::unique_ptr<ScriptsModel> machinePowerScriptsModel = std::make_unique<ScriptsModel>();
    std::unique_ptr<ModelView::SessionModel> treeModel     = std::make_unique<ScriptsTreeModel>();
    std::unique_ptr<ModelView::ViewModel> viewModel = ModelView::Factory::CreateTopItemsViewModel(
        treeModel.get());
    std::unique_ptr<ScriptsTreeProxyModel> proxyViewModel = std::make_unique<ScriptsTreeProxyModel>();
    std::unique_ptr<ScriptsModelIo> modelIo               = std::make_unique<ScriptsModelIo>();

    std::vector<std::unique_ptr<QTranslator>> translators{};

    ScriptsSnapIn *snapIn;

    std::string localeName = "en-US";

public:
    void retranslateModels(std::unique_ptr<ScriptsModel> &models);

public:
    ScriptsSnapInPrivate(ScriptsSnapIn *scriptsSnapIn);

public slots:
    void saveData();

    void reloadData();

private:
    ScriptsSnapInPrivate(const ScriptsSnapInPrivate &) = delete;      //copy ctor
    ScriptsSnapInPrivate(ScriptsSnapInPrivate &&)      = delete;      //move ctor
    ScriptsSnapInPrivate operator=(ScriptsSnapInPrivate &) = delete;  //copy assignment
    ScriptsSnapInPrivate operator=(ScriptsSnapInPrivate &&) = delete; //move assignment
};
} // namespace scripts_plugin

#endif // SCRIPTSSNAPINPRIVATE_H
