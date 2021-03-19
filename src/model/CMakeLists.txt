set(HEADERS
    checkbox.h
    combobox.h
    common.h
    decimaltextbox.h
    dropdownlist.h
    listbox.h
    longdecimaltextbox.h
    model.h
    multitextbox.h
    presentation.h
    presentationwidget.h
    presentationwidgetvisitor.h
    textbox.h
    text.h
)

set(SOURCES
    model.cpp
    presentationwidget.cpp
)

add_definitions(
    -DGPUI_MODEL_LIBRARY
)

add_gpui_library(gpui-model ${SOURCES})
target_link_libraries(gpui-model)
