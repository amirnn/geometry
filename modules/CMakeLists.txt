# modules


set(ARTIFACT_NAME_GEOMETRY geometry)

add_library(${ARTIFACT_NAME_GEOMETRY})
target_sources(${ARTIFACT_NAME_GEOMETRY}
        PRIVATE
        PUBLIC
        FILE_SET modules
        TYPE CXX_MODULES
        BASE_DIRS
        # interfaces
        include/la
        # partitions
        src/la
        FILES
        # geometry
        include/la/Geometry.ixx
        src/la/Vector.ixx
        src/la/Matrix.ixx
        src/la/Point.ixx
)