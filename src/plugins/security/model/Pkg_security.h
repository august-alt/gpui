#ifndef PKG_SECURITY
#define PKG_SECURITY

/************************************************************
 Pkg_security package header
 ************************************************************/

#ifndef _IN_
#define _IN_
#endif
#ifndef _OUT_
#define _OUT_
#endif
#ifndef _INOUT_
#define _INOUT_
#endif

/* Package dependency header include                        */

namespace security {

// Types defined within the package

/**
 * An enumeration of the 3 possible types a security definition can belong to.
 */
enum class SecurityType {
	/**
	 * 
	 */
	Machine = 1,
	/**
	 * 
	 */
	User = 0,
	/**
	 * 
	 */
	Both
};

} // of namespace security

/************************************************************
 End of Pkg_security package header
 ************************************************************/

#endif
