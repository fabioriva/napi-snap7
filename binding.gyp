{
  "targets": [
    {
      "target_name": "snap7_client",
      "sources": [
        "src/addon.cc",
        "src/snap7_client.cpp",
        "src/snap7.cpp"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "dependencies": ["<!(node -p \"require('node-addon-api').gyp\")", "snap7"],
      "cflags": [
        "-Wno-stringop-truncation",
        "-Wno-unused-but-set-variable",
        "-Wno-misleading-indentation",
        "-Wno-maybe-uninitialized",
        "-Wno-array-bounds",
        "-Wno-sign-compare",
        "-Wno-stringop-overread"
      ],
      "cflags!": [ "-fno-exceptions"],
      "cflags_cc!": [ "-fno-exceptions" ],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS"],
      "conditions": [
            ["OS=='win'", {
                "libraries": ["-lws2_32.lib", "-lwinmm.lib"],
                "defines": ["_WINSOCK_DEPRECATED_NO_WARNINGS", "_HAS_EXCEPTIONS=0"] # Make sure the STL doesn't try to use exceptions
            }]
        ],
    },
    {
      "target_name": "snap7",
      "type": "static_library",
      "sources": [
        "./deps/snap7/src/sys/snap_msgsock.cpp",
        "./deps/snap7/src/sys/snap_sysutils.cpp",
        "./deps/snap7/src/sys/snap_tcpsrvr.cpp",
        "./deps/snap7/src/sys/snap_threads.cpp",
        "./deps/snap7/src/core/s7_client.cpp",
        "./deps/snap7/src/core/s7_isotcp.cpp",
        "./deps/snap7/src/core/s7_partner.cpp",
        "./deps/snap7/src/core/s7_peer.cpp",
        "./deps/snap7/src/core/s7_server.cpp",
        "./deps/snap7/src/core/s7_text.cpp",
        "./deps/snap7/src/core/s7_micro_client.cpp",
        "./deps/snap7/src/lib/snap7_libmain.cpp"
       ],
      "include_dirs": [
        "./deps/snap7/src/sys",
        "./deps/snap7/src/core",
        "./deps/snap7/src/lib"
      ],
      "cflags": [
        "-Wno-stringop-truncation",
        "-Wno-unused-but-set-variable",
        "-Wno-misleading-indentation",
        "-Wno-maybe-uninitialized",
        "-Wno-array-bounds",
        "-Wno-sign-compare",
        "-Wno-stringop-overread"
      ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": ["-fno-exceptions"],
      "conditions": [
            ["OS=='linux' or OS=='freebsd'", {
                "cflags_cc": ["-fPIC", "-pedantic", "-fexceptions"],
                "cflags_cc!": ["-fno-exceptions"]
            }],
            ["OS=='win'", {
                "msvs_settings": {
                    "VCCLCompilerTool": {
                        "ExceptionHandling": 1,
                        "AdditionalOptions": ["/EHsc"] # ExceptionHandling=1 is not enough for some versions
                    }
                },
                "defines!": ["_HAS_EXCEPTIONS=0"],
                "defines": ["_WINSOCK_DEPRECATED_NO_WARNINGS"]
            }],
            ["OS=='mac'", {
                "xcode_settings": {
                    "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
                    "GCC_DYNAMIC_NO_PIC": "NO",
                    "OTHER_CFLAGS": ["-pedantic"]
                }
            }]
        ]
    }
  ]
}