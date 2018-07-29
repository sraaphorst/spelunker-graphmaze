# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

set(_GRAPHMAZE_PUBLIC_HEADER_FILES
        AldousBroderMazeGenerator.h
        BFSMazeGenerator.h
        BinaryTreeMazeGenerator.h
        DFSMazeGenerator.h
        GraphUtils.h
        HuntAndKillMazeGenerator.h
        MazeGraph.h
        MazeGenerator.h
        StringGridMazeRenderer.h
        PARENT_SCOPE
        )

set(_GRAPHMAZE_PRIVATE_HEADER_FILES
        PARENT_SCOPE
        )

set(_GRAPHMAZE_SOURCE_FILES
        AldousBroderMazeGenerator.cpp
        BinaryTreeMazeGenerator.cpp
        BFSMazeGenerator.cpp
        DFSMazeGenerator.cpp
        GraphUtils.cpp
        HuntAndKillMazeGenerator.cpp
        MazeGraph.cpp
        StringGridMazeRenderer.cpp
        PARENT_SCOPE
        )
