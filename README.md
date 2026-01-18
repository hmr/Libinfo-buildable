# Buildable Libinfo

🇯🇵 [日本語版 / Japanese](README.ja.md)

This is [Libinfo](https://github.com/apple-oss-distributions/Libinfo), a part of [Darwin](https://opensource.apple.com/releases/) released by Apple as open source software. Apple's OSS is designed to be built on an internal (non-public) SDK platform, so in most cases it cannot be built as-is on a standard macOS environment. To build it, you need to gather missing header files and modify the source code.

This repository contains those modifications, making it buildable on a regular macOS system.

## Versions

The software versions used are based on macOS 26.2.

```
Libinfo-repo/
├── Libinfo-600/            # https://github.com/apple-oss-distributions/Libinfo/archive/Libinfo-600.tar.gz
│   ├── AdditionalHeaders/  # Additional stub headers
│   ├── Libinfo.xcodeproj
│   ├── dns.subproj/
│   ├── gen.subproj/
│   ├── lookup.subproj/
│   ├── membership.subproj/
│   ├── nis.subproj/
│   ├── rpc.subproj/
│   ├── util.subproj/
│   └── xcodescripts/
│
└── Supplements/           # Sources for resolving dependencies
    ├── apple_internal_sdk/      # https://github.com/Torrekie/apple_internal_sdk
    ├── CoreOSMakefiles/         # https://github.com/apple-oss-distributions/CoreOSMakefiles/tree/CoreOSMakefiles-79
    ├── Libc-1725.40.4/          # https://github.com/apple-oss-distributions/Libc/archive/Libc-1725.40.4.tar.gz
    ├── Libsystem-1356/          # https://github.com/apple-oss-distributions/Libsystem/archive/Libsystem-1356.tar.gz
    ├── configd-1385.60.3/       # https://github.com/apple-oss-distributions/configd/archive/configd-1385.60.3.tar.gz
    ├── launchd-main/            # https://github.com/apple-oss-distributions/launchd
    ├── libdispatch-1542.0.4/    # https://github.com/apple-oss-distributions/libdispatch/archive/libdispatch-1542.0.4.tar.gz
    ├── mDNSResponder-2881.60.4/ # https://github.com/apple-oss-distributions/mDNSResponder/archive/mDNSResponder-2881.60.4.tar.gz
    └── xnu-12377.61.12/         # https://github.com/apple-oss-distributions/xnu/archive/xnu-12377.61.12.tar.gz
```

## Building

### Build Environment

- **macOS** + **Xcode (Command Line Tools)**

Build success has only been confirmed in my environment (macOS 15.7.3 / Xcode 26.2), but it should be buildable on considerably older versions of macOS/Xcode as well.

### How to Build

#### Command Line Build

```bash
cd Libinfo-600
xcodebuild -project Libinfo.xcodeproj -configuration Debug -target Libinfo clean build
```

### Build Artifacts

Upon successful build, the following variants will be generated in `build/Debug`:

- libsystem_info.dylib
- libsystem_info_debug.dylib
- libsystem_info_profile.dylib

## Debugging

The basic approach is to set the `DYLD_LIBRARY_PATH` environment variable to dynamically link your built library instead of the system's libsystem_info.dylib.

### Command Line Debugging

Prepare a binary that links against Libinfo:

```bash
DYLD_LIBRARY_PATH=./build/Debug lldb ./testbinary
```

### GUI Debugging

- Create a macOS Command Line Tool project in Xcode.
- Open [Menu Bar] - [Product] - [Scheme] - [Edit Scheme] (or ⌘ <).
  - Select [Run] from the left side menu of the settings window.
  - Select [Arguments] from the top right of the settings window.
  - In the [Environment Variables] section, set Name to `DYLD_LIBRARY_PATH` and Value to the full path to `build/Debug`.
  - Click the Close button.
- Add your debugging code to the project.
- After a successful build (⌘ B), set breakpoints at appropriate locations and Run (⌘ R).

## Technical Solutions

### Private Header Issues

The following measures were taken to address "non-existent private headers" included in the Libinfo source code:

- os/feature_private.h: Borrowed from Torrekie/apple_internal_sdk.
- opendirectory/od_private_hmr.h: Created by myself.
- Most others were gathered from other Apple OSS projects with the same filenames.
- Modified Libinfo.xcconfig to reference these headers.
- Headers that could not be found anywhere were commented out in the source code.
  - Build errors resulting from this were addressed by modifying the source code.

## Important Notes

⚠️ **Important**: This library is NOT intended to replace the system's `libsystem_info.dylib`.

- Use only for learning, research, and debugging purposes
- Stub headers are not complete implementations, and some features may not work
- The actual macOS system uses Apple's official libinfo

## Known Issues

- Some functions generate warnings due to `-Wdeprecated-declarations`. Please ignore them.
- `xcodescripts/install_files.sh` runs every time you run xcodebuild. This causes header files to be copied to /tmp/Libinfo.dst/ each time. There is no practical harm, so this is left as-is.
- Due to the same cause as the previous issue, `xcodebuild clean` fails to delete /tmp/Libinfo.dst/ and reports an error. Other build artifacts are deleted, and since there is no practical harm, this is left as-is.

## License

- Apple OSS is licensed under the `Apple Public Source License (APSL)`.
  - APSL has versions 1.0, 1.1, 1.2, and 2.0, with somewhat different terms.
  - Different software uses different versions.
- The license for apple_internal_sdk is not explicitly stated.

## Related Resources

- [Apple Open Source](https://opensource.apple.com/source/)
- [Apple OSS Distributions](https://github.com/apple-oss-distributions)
- [Torrekie/apple_internal_sdk](https://github.com/Torrekie/apple_internal_sdk)
- [showxu/objc4](https://github.com/showxu/objc4)
- [The Unofficial Guide to xcconfig files](https://pewpewthespells.com/blog/xcconfig_guide.html)
- [Building xnu for OS X 10.11 El Capitan (ssen's blog)](https://shantonu.blogspot.com/2015/12/building-xnu-for-os-x-1011-el-capitan.html)