set(HEADERS
    common.h
    model.h
    presentation/checkbox.h
    presentation/combobox.h
    presentation/decimaltextbox.h
    presentation/dropdownlist.h
    presentation/listbox.h
    presentation/longdecimaltextbox.h
    presentation/multitextbox.h
    presentation/presentation.h
    presentation/presentationwidget.h
    presentation/presentationwidgetvisitor.h
    presentation/textbox.h
    presentation/text.h
)

set(SOURCES
    model.cpp
    presentation/presentationwidget.cpp
)

add_definitions(
    -DGPUI_MODEL_LIBRARY
)

add_gpui_library(gpui-model ${SOURCES})
target_link_libraries(gpui-model)
