#ifndef COMPILER_H
#define COMPILER_H

/* ===================== VERSION INFO ===================== */
#define COMPILER_AR_RELEASE_MAJOR_VERSION 4U
#define COMPILER_AR_RELEASE_MINOR_VERSION 3U
#define COMPILER_AR_RELEASE_PATCH_VERSION 1U

#define COMPILER_SW_MAJOR_VERSION 1U
#define COMPILER_SW_MINOR_VERSION 0U
#define COMPILER_SW_PATCH_VERSION 0U

/* ===================== MEMORY QUALIFIERS ===================== */

/* Function declaration */
#define FUNC(rettype, memclass) rettype

/* Pointer to variable */
#define P2VAR(ptrtype, memclass, ptrclass) ptrtype *

/* Pointer to constant */
#define P2CONST(ptrtype, memclass, ptrclass) const ptrtype *

/* Constant pointer to variable */
#define CONSTP2VAR(ptrtype, memclass, ptrclass) ptrtype *const

/* Constant pointer to constant */
#define CONSTP2CONST(ptrtype, memclass, ptrclass) const ptrtype *const

/* Pointer to function */
#define P2FUNC(rettype, ptrclass, fctname) rettype(*fctname)

/* Function pointer typedef */
#define FUNC_P2FUNC(rettype, ptrclass) rettype(*)

/* ===================== VARIABLE QUALIFIERS ===================== */

/* Variable in RAM */
#define VAR(vartype, memclass) vartype

/* Constant */
#define CONST(consttype, memclass) const consttype

/* Static variable */
#define STATIC static

/* ===================== MEMORY SECTION MAPPING ===================== */

/* These are used with MemMap.h */
#define START_SEC_CODE
#define STOP_SEC_CODE

#define START_SEC_VAR_INIT
#define STOP_SEC_VAR_INIT

#define START_SEC_VAR_ZERO_INIT
#define STOP_SEC_VAR_ZERO_INIT

#define START_SEC_CONST
#define STOP_SEC_CONST

#include "MemMap.h"

/* ===================== INLINE HANDLING ===================== */

#ifndef INLINE
#define INLINE static inline
#endif

#ifndef LOCAL_INLINE
#define LOCAL_INLINE static inline
#endif

/* ===================== NULL POINTER ===================== */

#ifndef NULL_PTR
#define NULL_PTR ((void *)0)
#endif

/* ===================== COMPILER SPECIFIC FIXES ===================== */

/* Example GCC attributes (optional) */
#if defined(__GNUC__)
#define COMPILER_WEAK __attribute__((weak))
#define COMPILER_ALIGNED(x) __attribute__((aligned(x)))
#else
#define COMPILER_WEAK
#define COMPILER_ALIGNED(x)
#endif

/* ===================== FUNCTION MACROS ===================== */

#define FUNC_DECL(rettype, memclass) rettype
#define FUNC_STATIC(rettype, memclass) static rettype

#endif /* COMPILER_H */