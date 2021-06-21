/***********************************************************************************************************************
**
** Copyright (C) 2021 BaseALT Ltd.
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**
***********************************************************************************************************************/

#ifndef GPUI_DOWNLOAD_ADMX_DIALOG_H
#define GPUI_DOWNLOAD_ADMX_DIALOG_H

#include <QDialog>

#include "gui.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DownloadDefinitionsDialog; }
QT_END_NAMESPACE

class QFileInfo;

namespace gpui {

    class DownloadADMXDialogPrivate;

    class GPUI_GUI_EXPORT DownloadADMXDialog : public QDialog {
        Q_OBJECT

    public:
        // construction and destruction
        DownloadADMXDialog(QWidget *parent = 0);
        ~DownloadADMXDialog();

    private:

        DownloadADMXDialogPrivate* const d;

        Ui::DownloadDefinitionsDialog *ui;

    private:
        void downloadFiles(const QString& url, const QFileInfo& outputFolder);

        void unpackMSI(const QString& inputMSI, const QFileInfo& outputFolder);

    private slots:
        void onBrowseClick();

        void onOKClick();
    };
}

#endif // GPUI_DOWNLOAD_ADMX_DIALOG_H
