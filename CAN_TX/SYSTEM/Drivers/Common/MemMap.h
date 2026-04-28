#ifndef MEMMAP_H
#define MEMMAP_H

/*
 * AUTOSAR Memory Mapping concept
 * --------------------------------
 * This file is used to map code/data into memory sections
 * defined by linker script.
 */

/* ===================== CODE SECTION ===================== */

#ifdef START_SEC_CODE
#undef START_SEC_CODE
// #pragma push
// #pragma section code "CODE"
#endif

#ifdef STOP_SEC_CODE
#undef STOP_SEC_CODE
// #pragma pop
#endif

/* ===================== CONSTANT SECTION ===================== */

#ifdef START_SEC_CONST
#undef START_SEC_CONST
// #pragma push
// #pragma section const "CONST"
#endif

#ifdef STOP_SEC_CONST
#undef STOP_SEC_CONST
// #pragma pop
#endif

/* ===================== INIT DATA SECTION ===================== */

#ifdef START_SEC_VAR_INIT
#undef START_SEC_VAR_INIT
// #pragma push
// #pragma section data "DATA"
#endif

#ifdef STOP_SEC_VAR_INIT
#undef STOP_SEC_VAR_INIT
// #pragma pop
#endif

/* ===================== ZERO INIT SECTION ===================== */

#ifdef START_SEC_VAR_ZERO_INIT
#undef START_SEC_VAR_ZERO_INIT
// #pragma push
// #pragma section bss "BSS"
#endif

#ifdef STOP_SEC_VAR_ZERO_INIT
#undef STOP_SEC_VAR_ZERO_INIT
// #pragma pop
#endif

#endif /* MEMMAP_H */