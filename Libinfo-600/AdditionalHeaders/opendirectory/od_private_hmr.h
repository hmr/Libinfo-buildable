/*
 * Stub header to pass build for Libinfo on non-macos.internal SDK
 *
 * Copyright (c) 2026 hmr. All rights reserved.
 */

#ifndef _OD_PRIVATE_H_
#define _OD_PRIVATE_H_

// Open Directory Mach port names - Membership
#ifndef kODMachMembershipPortName
#define kODMachMembershipPortName "com.apple.system.opendirectoryd.membership"
#endif

#ifndef kODMachMembershipPortNameDebug
#define kODMachMembershipPortNameDebug "com.apple.system.opendirectoryd.membership.debug"
#endif

// Open Directory Mach port names - Libinfo
#ifndef kODMachLibinfoPortName
#define kODMachLibinfoPortName "com.apple.system.opendirectoryd.libinfo"
#endif

#ifndef kODMachLibinfoPortNameDebug
#define kODMachLibinfoPortNameDebug "com.apple.system.opendirectoryd.libinfo.debug"
#endif

// DirectoryService Mach port name (legacy)
#ifndef kDSStdMachDSLookupPortName
#define kDSStdMachDSLookupPortName "com.apple.system.DirectoryService.libinfo_v1"
#endif

// XPC pipe flags (from xpc/private.h)
#ifndef XPC_PIPE_PRIVILEGED
#define XPC_PIPE_PRIVILEGED (1 << 0)
#endif

#ifndef XPC_PIPE_PROPAGATE_QOS
#define XPC_PIPE_PROPAGATE_QOS (1 << 1)
#endif

// Open Directory RPC keys
#ifndef OD_RPC_NAME
#define OD_RPC_NAME "name"
#endif

#ifndef OD_RPC_VERSION
#define OD_RPC_VERSION "version"
#endif

#ifndef OD_RPC_RESULT
#define OD_RPC_RESULT "result"
#endif

#ifndef OD_RPC_ERROR
#define OD_RPC_ERROR "error"
#endif

#endif /* _OD_PRIVATE_H_ */
