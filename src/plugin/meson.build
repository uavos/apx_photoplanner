qt5_dep = dependency('qt5', 
                      modules: ['Core', 'Widgets', 'Gui', 'Quick', 'Qml', 'Network'])

cpp = meson.get_compiler('cpp')
apx_sdk = '/home/pavel/apx_sdk'
apx_sdk_lib = apx_sdk / 'lib'
apx_sdk_inc = [apx_sdk / 'include/ApxCore',
               apx_sdk / 'include/ApxData',
               apx_sdk / 'include/ApxGcs',
               apx_sdk / 'include/ApxShared']
apx_libs = [cpp.find_library('ApxCore', dirs: [apx_sdk_lib]),
            cpp.find_library('ApxData', dirs: [apx_sdk_lib]),
            cpp.find_library('ApxGcs', dirs: [apx_sdk_lib]),
            cpp.find_library('ApxShared', dirs: [apx_sdk_lib])]

qt5 = import('qt5')

moc_sources = qt5.compile_moc(headers: ['cameraedit.h',
                                        'photoplannerplugin.h',
                                        'photoplanner.h',
                                        'borderpoints.h',
                                        'photoprints.h',
                                        'planeredit.h',
                                        'pointedit.h',
                                        'uavedit.h'],
                             include_directories: include_directories(apx_sdk_inc))

qrc_sources = qt5.compile_resources(name: '',
                                    sources: ['stuff.qrc'],
                                    method: 'qmake')
                                      
plugin_lib = shared_library('apx_photoplanner',
                            'cameraedit.cpp',
                            'photoplannerplugin.cpp',
                            'photoplanner.cpp',
                            'borderpoints.cpp',
                            'photoprints.cpp',
                            'planeredit.cpp',
                            'pointedit.cpp',
                            'uavedit.cpp',
                             moc_sources,
                             qrc_sources,
                             dependencies: [apx_libs,
                                            math_dep,
                                            qt5_dep],
                             include_directories: include_directories(apx_sdk_inc),
                             install_dir: '', 
                             install: true)