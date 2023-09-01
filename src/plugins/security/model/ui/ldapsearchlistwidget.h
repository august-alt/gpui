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

#ifndef SECURITY_LDAP_SEARCH_LIST_WIDGET_H
#define SECURITY_LDAP_SEARCH_LIST_WIDGET_H

#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class LdapSearchListWidget; }
QT_END_NAMESPACE

namespace security
{

class LdapSearchListWidgetPrivate;

class LdapSearchListWidget : public QWidget
{
public:
    Q_OBJECT

public:
    explicit LdapSearchListWidget(const QString& filter, const QString& dn, QWidget* parent = nullptr);
    explicit LdapSearchListWidget(const std::string& filter, const std::string& dn, QWidget* parent = nullptr);
    ~LdapSearchListWidget() override;

    void setAddButtonText(const QString& text);
    void setRemoveButtonText(const QString& text);

private slots:
    void on_addButton_clicked();
    void on_removeButton_clicked();

private:
    LdapSearchListWidget(const LdapSearchListWidget&)            = delete;   // copy ctor
    LdapSearchListWidget(LdapSearchListWidget&&)                 = delete;   // move ctor
    LdapSearchListWidget& operator=(const LdapSearchListWidget&) = delete;   // copy assignment
    LdapSearchListWidget& operator=(LdapSearchListWidget&&)      = delete;   // move assignment

private:
    LdapSearchListWidgetPrivate *d{};

private:
    Ui::LdapSearchListWidget *ui{};
};

}

#endif//SECURITY_LDAP_SEARCH_LIST_WIDGET_H
