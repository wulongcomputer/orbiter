# Copyright (c) Martin Schweiger
# Licensed under the MIT License

add_executable(Date
	Date.cpp
	Date.rc
	DateDlg.cpp
	Convert.cpp
	StdAfx.cpp
)

set_target_properties(Date
	PROPERTIES
	FOLDER Tools
)

add_definitions(-D_AFXDLL)

set_target_properties(Date
	PROPERTIES
	LINK_FLAGS "/SUBSYSTEM:WINDOWS"
	FOLDER Utils
)

install(TARGETS
	Date
	RUNTIME
	DESTINATION ${ORBITER_INSTALL_UTILS_DIR}
)
