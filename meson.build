project('qmlsink-dynamically-added', 'cpp')

sources = [
  'main.cpp',
]

# Dependency declarations (ensure paths are correct)
gst_dep = dependency('gstreamer-1.0', required: true)
gstgl_dep = dependency('gstreamer-gl-1.0', required: true)
qt5qml_example_deps = dependency('qt5qml', required: true)

qt_preprocessed = qt5_mod.preprocess(qresources : 'qmlsink-dyn-added.qrc')

# Executable declaration
executable('qmlsink-dynamically-added', 
    sources,
    qt_preprocessed,
    dependencies : [gst_dep, gstgl_dep, qt5qml_example_deps],
    override_options : ['cpp_std=c++11'],
    c_args : gst_plugins_good_args,
    include_directories : [configinc],
    install: false)
