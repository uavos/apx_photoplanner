qt5_dep = dependency('qt5', 
                      modules: ['Core', 'Gui', 'Positioning'])
                                      
math_lib = static_library('math',
                          'AreaPhotoPlanner.cpp',
                          'AreaPhotoPrintsGenerator.cpp',
                          'LinearPhotoPlanner.cpp',
                          'LinearPhotoPrintsGenerator.cpp',
                          'LinedGeoPoints.cpp',
                          'LinePhotoPrintsGenerator.cpp',
                          'ManeuverTrackAlignment.cpp',
                          'PhotoCameraModel.cpp',
                          'PhotoGeoCalcs.cpp',
                          'PhotoPlanner.cpp',
                          'PhotoPlannerCore.cpp',
                          'PhotoPlannerXmlWriter.cpp',
                          'PhotoUavModel.cpp',
                           dependencies: [qt5_dep],
                           install: false)

math_dep = declare_dependency(link_with: math_lib,
                              include_directories: include_directories('.'),
                              dependencies: [qt5_dep])