add_subdirectory(figures)

add_subdirectory(common)
add_subdirectory(widgets)

add_library(draw_lib STATIC
        Canvas.cpp
        PolyBuilder.cpp
        CanvasBuilder.cpp
        Canvas3DViewer.cpp
        Axis.cpp
        DrawStyles.cpp
        )

target_link_libraries(draw_lib
        figures_lib
        widgets_lib
        layout_manager_lib)