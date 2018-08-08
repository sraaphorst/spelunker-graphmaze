# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

set(_GRAPHMAZE_PUBLIC_HEADER_FILES
        AldousBroderMazeGenerator.h
        BFSMazeGenerator.h
        BinaryTreeMazeGenerator.h
        CircularTemplates.h
        DFSMazeGenerator.h
        GraphUtils.h
        GridTemplates.h
        HuntAndKillMazeGenerator.h
        MazeGraph.h
        MazeGenerator.h
        OrthogonalTemplates.h
        RoundTemplates.h
        SidewinderMazeGenerator.h
        SphericalTemplates.h
        StringGridMazeRenderer.h
        UpsilonTemplates.h
        ZetaTemplates.h
        PARENT_SCOPE
        )

set(_GRAPHMAZE_PRIVATE_HEADER_FILES
        PARENT_SCOPE
        )

set(_GRAPHMAZE_SOURCE_FILES
        AldousBroderMazeGenerator.cpp
        BinaryTreeMazeGenerator.cpp
        BFSMazeGenerator.cpp
        CircularTemplates.cpp
        DFSMazeGenerator.cpp
        GraphUtils.cpp
        GridTemplates.cpp
        HuntAndKillMazeGenerator.cpp
        MazeGraph.cpp
        OrthogonalTemplates.cpp
        RoundTemplates.cpp
        SidewinderMazeGenerator.cpp
        SphericalTemplates.cpp
        StringGridMazeRenderer.cpp
        UpsilonTemplates.cpp
        ZetaTemplates.cpp
        PARENT_SCOPE
        )
