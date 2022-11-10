#ifndef LDAPIMPL_H
#define LDAPIMPL_H

#include "ad_config.h"
#include "ad_interface.h"
#include "ldap-common.h"
#include "ldapcontract.h"

#include <memory>

namespace ldap
{
class LDAPImplPrivate;

class GPUI_LDAP_EXPORT LDAPImpl : public LDAPContract
{
public:
    LDAPImpl();

    virtual ~LDAPImpl() = default;

    /**
     * @brief initialize ldap communication
     * @return true - if it success, false - if it fails
     */
    virtual bool initialize();

    /**
     * @brief getDisplayNameGPO
     * @param dn destinquish name
     * @return std::string with display name of GPO
     */
    virtual QString getDisplayNameGPO(const QString &guid);

    virtual const QHash<QString, AdObject> search(const QString &base,
                                                  const SearchScope scope,
                                                  const QString &filter,
                                                  const QList<QString> &attributes,
                                                  const bool get_sacl = false);

    virtual AdConfig *getAdConfig();

    virtual QString getFilterCondition(const LDAPContract::Condition condition,
                                       const QString &attribute,
                                       const QString &value = QString());

    virtual QString getFilter_AND(const QList<QString> &subfilters);

    virtual QString getFilter_OR(const QList<QString> &subfilters);

    virtual QString getDCName();

private:
    std::unique_ptr<LDAPImplPrivate> d;

    LDAPImpl(const LDAPImpl &) = delete;            //copy ctor
    LDAPImpl(LDAPImpl &&)      = delete;            //move ctor
    LDAPImpl &operator=(const LDAPImpl &) = delete; //copy assignment
    LDAPImpl &operator=(LDAPImpl &&) = delete;      //move assignment
};

} //namespace ldap

#endif // LDAPIMPL_H
