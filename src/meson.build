src = [ 'main.c' ]
gst_dep = dependency('gstreamer-1.0', method : 'pkg-config')
gst_gl_dep = dependency('gstreamer-gl-1.0', method : 'pkg-config')
thread_dep = dependency('threads')

executable('analysis_test', sources : src, dependencies : [gst_dep, gst_gl_dep, thread_dep])
