# nfer 1.1.3
 * Added support for named constants in the DSL.
 * Some code restructuring to improve maintainability.

# nfer 1.1.2

 * Add constant folding.
 * Fix a problem with C23 support caused by defining bool that caused nfer to be archived on CRAN.
 * Add a silent import feature to the DSL.
 * Fix a problem that prevented nested imports from working.
 * Fix support for the MS VS compiler.
 * Replace all calls to sprintf with snprintf.
 * Fix a warning from casting pointers to ints when they should have been cast to int pointers.

# nfer 1.1.1

 * Fix a tricky bug where intervals could exclude themselves.
 * Fix a bug where single-rule cycles were not correctly iterated on.

# nfer 1.1.0

 * This release includes some minor changes to the nfer language semantics and 
   brings the implementation more closely in line with it.  The changes to
   the semantics are to fix a problem where exclusive rules could be 
   non-deterministic when they depended on other rules.
 * The release also includes some bug fixes and performance improvements.

# nfer 1.0.1

 * Fixing build errors under Solaris 10 ODS.

# nfer 1.0.0

 * First official release of the nfer package!