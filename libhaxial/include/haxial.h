/**
 * @file haxial.h
 * @brief Main header file for libhaxial
 * 
 * This header includes all public API headers for the libhaxial library.
 * Binary-compatible reimplementation of Haxial KDX protocol.
 */

#ifndef HAXIAL_H
#define HAXIAL_H

/* Version information */
#define LIBHAXIAL_VERSION_MAJOR 0
#define LIBHAXIAL_VERSION_MINOR 1
#define LIBHAXIAL_VERSION_PATCH 0
#define LIBHAXIAL_VERSION "0.1.0"

/* Core headers */
#include "haxial/errors.h"

/* Cryptography */
#include "haxial/sha1.h"
#include "haxial/blowfish.h"

/* Data structures */
#include "haxial/list.h"
#include "haxial/pstring.h"

/* Utilities */
#include "haxial/fnv1a.h"
#include "haxial/random.h"

/* TODO: Add more headers as we implement them */
/* #include "haxial/socket.h" */
/* #include "haxial/connection.h" */
/* #include "haxial/packet.h" */
/* #include "haxial/protocol.h" */

#endif /* HAXIAL_H */
