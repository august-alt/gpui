#include "ldapcontract.h"

namespace ldap {

LDAPContract::LDAPContract() : errorStr(""), connectedFlag(false)
{
}

LDAPContract::~LDAPContract()
{

}

std::string LDAPContract::getError()
{
    return errorStr;
}

bool LDAPContract::isConnected()
{
    return connectedFlag;
}

void LDAPContract::setConnect(bool connected)
{
    connectedFlag = connected;
}

void LDAPContract::setError(std::string er)
{
    errorStr = er;
}


} //namespace ldap
