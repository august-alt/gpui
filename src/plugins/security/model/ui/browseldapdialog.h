/***********************************************************************************************************************
**
** Copyright (C) 2023 BaseALT Ltd. <org@basealt.ru>
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

#ifndef SECURITY_BROWSE_LDAP_DIALOG_H
#define SECURITY_BROWSE_LDAP_DIALOG_H

#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class BrowseLdapDialog; }
QT_END_NAMESPACE

namespace security
{

class BrowseLdapDialogPrivate;

class BrowseLdapDialog : public QDialog
{
public:
    Q_OBJECT

public:
    explicit BrowseLdapDialog(QWidget* parent = nullptr);
    ~BrowseLdapDialog() override;

private:
    BrowseLdapDialog(const BrowseLdapDialog&)            = delete;   // copy ctor
    BrowseLdapDialog(BrowseLdapDialog&&)                 = delete;   // move ctor
    BrowseLdapDialog& operator=(const BrowseLdapDialog&) = delete;   // copy assignment
    BrowseLdapDialog& operator=(BrowseLdapDialog&&)      = delete;   // move assignment

private:
    BrowseLdapDialogPrivate *d{};

private:
    Ui::BrowseLdapDialog *ui{};
};

}

#endif//SECURITY_BROWSE_LDAP_DIALOG_H
