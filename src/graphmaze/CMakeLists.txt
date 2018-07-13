# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

set(_GRAPHMAZE_PUBLIC_HEADER_FILES
        AldousBroderMazeGenerator.h
        BFSMazeGenerator.h
        DFSMazeGenerator.h
        GraphUtils.h
        HuntAndKillMazeGenerator.h
        MazeGraph.h
        MazeGenerator.h
        PARENT_SCOPE
        )

set(_GRAPHMAZE_PRIVATE_HEADER_FILES
        PARENT_SCOPE
        )

set(_GRAPHMAZE_SOURCE_FILES
        AldousBroderMazeGenerator.cpp
        BFSMazeGenerator.cpp
        DFSMazeGenerator.cpp
        GraphUtils.cpp
        HuntAndKillMazeGenerator.cpp
        PARENT_SCOPE
        )
