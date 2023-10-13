#include "ldapimpl.h"
#include "ad_filter.h"

#include <QLocale>

#define ATTRIBUTE_GPC_MACHINE_EXTENSION_NAMES "gPCMachineExtensionNames"
#define ATTRIBUTE_GPC_USER_EXTENSION_NAMES    "gPCUserExtensionNames"

namespace ldap {

class LDAPImplPrivate {
public:
    std::unique_ptr<AdInterface> adInterface;
    std::unique_ptr<AdConfig> adConfig;

    LDAPImplPrivate() : adInterface(std::make_unique<AdInterface>())
      , adConfig(std::make_unique<AdConfig>())
    {}
};

LDAPImpl::LDAPImpl() : LDAPContract(), d(new LDAPImplPrivate())
{
    if(d->adInterface.get()->is_connected()) {
        setConnect(true);

        const QLocale locale = QLocale::system().language();

        d->adConfig.get()->load(*d->adInterface.get(), locale);

        d->adInterface.get()->set_config(d->adConfig.get());

    }
}

bool LDAPImpl::initialize()
{
    return false;
}

QString LDAPImpl::getDisplayNameGPO(const QString &guid)
{
    const QString base = d->adConfig.get()->policies_dn();
    const SearchScope scope = SearchScope_All;

    QString filter = getFilterCondition(ldap::LDAPContract::Condition::Condition_Equals, ATTRIBUTE_OBJECT_CLASS, CLASS_GP_CONTAINER);
    QString filter2 = getFilterCondition(ldap::LDAPContract::Condition::Condition_Equals, ATTRIBUTE_GPC_FILE_SYS_PATH, "*" + guid + "*");

    QList<QString> filtersList;
    filtersList << filter << filter2;
    QString commonFilter = getFilter_AND(filtersList);

    const QList<QString> attributes = QList<QString>();
    const QHash<QString, AdObject> results = search(base, scope, commonFilter, attributes);

    if(results.size() > 0)
    {
        QList<QString> keys = results.keys();

        return results[keys[0]].get_strings("displayName")[0];

    }

    return NULL;
}

int LDAPImpl::getGPOVersion(const QString &guid)
{
    const QString base = d->adConfig.get()->policies_dn();
    const SearchScope scope = SearchScope_All;

    QString filter = getFilterCondition(ldap::LDAPContract::Condition::Condition_Equals, ATTRIBUTE_OBJECT_CLASS, CLASS_GP_CONTAINER);
    QString filter2 = getFilterCondition(ldap::LDAPContract::Condition::Condition_Equals, ATTRIBUTE_GPC_FILE_SYS_PATH, "*" + guid + "*");

    QList<QString> filtersList;
    filtersList << filter << filter2;
    QString commonFilter = getFilter_AND(filtersList);

    const QList<QString> attributes = QList<QString>();
    const QHash<QString, AdObject> results = search(base, scope, commonFilter, attributes);

    if(results.size() > 0)
    {
        QList<QString> keys = results.keys();

        return results[keys[0]].get_strings(ATTRIBUTE_VERSION_NUMBER)[0].toInt();

    }

    return 0;
}

const QHash<QString, AdObject> LDAPImpl::search(const QString &base, const SearchScope scope, const QString &filter, const QList<QString> &attributes, const bool get_sacl)
{
    Q_UNUSED(get_sacl);
    QHash<QString, AdObject> result;

    if(!d->adInterface->is_connected()) {
        return result;
    }

    return d->adInterface.get()->search(base, scope, filter, attributes);
}

bool LDAPImpl::setExtensions(const QString &guid, const QString& machineExtensions, const QString& userExtensions, const int machineVersion, const int userVersion)
{
    const QString gpc_dn = QString("CN={%1},CN=Policies,CN=System,%2").arg(guid, d->adConfig.get()->policies_dn());

    const int version = (userVersion << 16) + machineVersion;

    d->adInterface->attribute_replace_string(gpc_dn, ATTRIBUTE_GPC_MACHINE_EXTENSION_NAMES, machineExtensions);

    d->adInterface->attribute_replace_string(gpc_dn, ATTRIBUTE_GPC_USER_EXTENSION_NAMES, userExtensions);

    d->adInterface->attribute_replace_int(gpc_dn, ATTRIBUTE_VERSION_NUMBER, version);

    return true;
}

AdConfig* LDAPImpl::getAdConfig()
{
    return d->adConfig.get();
}

QString LDAPImpl::getFilterCondition(const LDAPContract::Condition condition, const QString &attribute, const QString &value)
{
    switch (condition) {
        case Condition_Equals: return QString("(%1=%2)").arg(attribute, value);
        case Condition_NotEquals: return QString("(!(%1=%2))").arg(attribute, value);
        case Condition_StartsWith: return QString("(%1=%2*)").arg(attribute, value);
        case Condition_EndsWith: return QString("(%1=*%2)").arg(attribute, value);
        case Condition_Contains: return QString("(%1=*%2*)").arg(attribute, value);
        case Condition_Set: return QString("(%1=*)").arg(attribute);
        case Condition_Unset: return QString("(!(%1=*))").arg(attribute);
        case Condition_COUNT: return QString();
    }
    return QString();
}

QString LDAPImpl::getFilter_AND(const QList<QString> &subfilters)
{
    return filter_AND(subfilters);

}

QString LDAPImpl::getFilter_OR(const QList<QString> &subfilters)
{
    return filter_OR(subfilters);
}


QString LDAPImpl::getDCName()
{
    return d->adInterface.get()->getDCName();
}

} //namespace ldap
