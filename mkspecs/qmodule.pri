EXTRA_INCLUDEPATH += /home/qt/openssl-1.0.2/include
host_build {
    QT_CPU_FEATURES.x86_64 = mmx sse sse2
} else {
    QT_CPU_FEATURES.i386 = mmx sse sse2 sse3 ssse3
}
QT.global_private.enabled_features = sse2 alloca_h alloca android-style-assets avx2 gui network qml-debug reduce_exports reduce_relocations sql system-zlib testlib widgets xml
QT.global_private.disabled_features = alloca_malloc_h private_tests dbus dbus-linked libudev posix_fallocate release_tools stack-protector-strong
QT_COORD_TYPE = double
QMAKE_LIBS_ZLIB = -lz
CONFIG -= precompile_header
CONFIG += cross_compile sse2 aesni sse3 ssse3 sse4_1 sse4_2 avx avx2 avx512f avx512cd avx512er avx512pf compile_examples f16c rdrnd shani
QT_BUILD_PARTS += libs
