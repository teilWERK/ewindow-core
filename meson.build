project('ewindow', 'cpp')

qt5 = import('qt5')
qt5_dep = dependency('qt5', modules: ['Core', 'Quick'])

qwebrtc = subproject('qwebrtc')
qwebrtc_dep = qwebrtc.get_variable('qwebrtc_dep')

qtzeroconf = subproject('QtZeroConf')
qtzeroconf_dep = qtzeroconf.get_variable('qtzeroconf_dep')

compiler = meson.get_compiler('cpp')
libgl = compiler.find_library('GL')

webrtc_dep = dependency('webrtc', fallback: ['webrtc', 'webrtc_dep'])

moc_headers = files(
    'peerfinder.h',
    'peerlist.h',
    'coolsocket.h',
    'simplepeerconnection.h'
    #'baresipcore.h',
    #'baresipvidisp.h',
)

sources = files(
    'main.cpp',
    'peerfinder.cpp',
    'peerlist.cpp',
    'coolsocket.cpp',
    'simplepeerconnection.cpp'
    #'baresipcore.cpp',
    #'baresipvidisp.cpp',
    #'yuvtexturematerial.cpp',
)

moc_files = qt5.preprocess(moc_headers: moc_headers)

executable('ewindow', sources, moc_files,
    dependencies: [qtzeroconf_dep, qt5_dep, qwebrtc_dep, webrtc_dep],
    cpp_args: '-DQT_NO_KEYWORDS',
    link_args: '-lGL' # Hack, should use Qt's OpenGL wrapper
)
