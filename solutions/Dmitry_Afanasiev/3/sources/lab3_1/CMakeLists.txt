

find_package( Boost ${SEARCH_PARAMETERS} COMPONENTS
	system
	thread bind 
)

compile_project( ${module_name} "*.cpp" "*.h" BINARY binaries Boost )

