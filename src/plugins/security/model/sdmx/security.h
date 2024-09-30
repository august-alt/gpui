#ifndef SECURITY_H
#define SECURITY_H

namespace security
{

/**
 * \enum SecurityType
 * \brief bAn enumeration of the 3 possible types a security definition can belong to.
 */
enum class SecurityType
{
    /*!
     *
     */
    User = 0,

    /*!
     *
     */
    Machine = 1,

    /*!
     *
     */
    Both = 2
};

} // of namespace security

#endif // SECURITY_H
