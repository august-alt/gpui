/***********************************************************************************************************************
**
** Copyright (C) 2021 BaseALT Ltd. <org@basealt.ru>
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

#ifndef GPUI_LDAPCONTRACT_H
#define GPUI_LDAPCONTRACT_H

#include <QString>
#include <string>
#include <QHash>

#include "ldap-common.h"
#include "ad_object.h"


namespace ldap {

class GPUI_LDAP_EXPORT LDAPContract {
public:
    enum Condition {
        Condition_Contains,
        Condition_Equals,
        Condition_NotEquals,
        Condition_StartsWith,
        Condition_EndsWith,
        Condition_Set,
        Condition_Unset,
        Condition_COUNT,
    };

private:

    std::string errorStr;

    bool connectedFlag;

protected:

    void setError(std::string);

    void setConnect(bool connected);

public:

    LDAPContract();

    virtual ~LDAPContract();

    virtual bool initialize() = 0;

    /**
     * @brief getDisplayNameGPO returns displayName attribute of LDAP object
     * @param guid QString with GUID
     * @return return QSrting with displayName attribute or NULL, if error
     */
    virtual QString getDisplayNameGPO(const QString &guid) = 0;

    virtual const QHash<QString, AdObject> search(const QString &base, const SearchScope scope, const QString &filter, const QList<QString> &attributes, const bool get_sacl = false) = 0;

    virtual AdConfig* getAdConfig() = 0;

    virtual QString getFilterCondition(const LDAPContract::Condition condition, const QString &attribute, const QString &value = QString()) = 0;

    virtual QString getFilter_AND(const QList<QString> &subfilters) = 0;

    virtual QString getFilter_OR(const QList<QString> &subfilters) = 0;

    virtual QString getDCName() = 0;

    std::string getError();

    bool isConnected();

};


} //namespace ldap

#endif //GPUI_LDAPCONTRACT_H
