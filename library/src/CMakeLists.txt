set( PROTO_GEN_DIR ${CMAKE_BINARY_DIR}/gen )
file( MAKE_DIRECTORY ${PROTO_GEN_DIR} )

set( WSH_PROTOC_SOURCES ${PROTO_GEN_DIR}/cmd-messages.pb-c.c ${PROTO_GEN_DIR}/auth.pb-c.c )
set( WSH_SOURCES log.c cmd.c pack.c ssh.c )

if( LIBCRANGE_LIBRARY )
	set( WSH_SOURCES ${WSH_SOURCES} range_expansion.c )
endif( LIBCRANGE_LIBRARY )

add_library( wsh SHARED ${WSH_PROTOC_SOURCES} ${WSH_SOURCES} )

# Find protoc-c compiler
find_program( PROTO_C protoc-c )
set( PROTOC_C_C_OUT_FLAG --c_out )
set( PROTOC_C_PROTO_PATH --proto_path )

# Grabs all .proto files in current dir
file( GLOB PROTO_INPUT RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.proto )

# Loops over proto files and changes the internal name to compiled names
foreach( PROTO_FILE ${PROTO_INPUT} )
	get_filename_component( PROTO_NAME ${PROTO_FILE} NAME_WE )
	set( PROTO_GEN ${PROTO_GEN} 
		${PROTO_GEN_DIR}/${PROTO_NAME}.pb-c.h 
		${PROTO_GEN_DIR}/${PROTO_NAME}.pb-c.c )
endforeach( PROTO_FILE ${PROTO_INPUT} )

add_custom_command(
	OUTPUT ${PROTO_GEN}
	COMMAND ${PROTO_C} ${PROTO_INPUT} ${PROTOC_C_C_OUT_FLAG} ${PROTO_GEN_DIR}
	DEPENDS ${PROTO_C}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )

set_source_files_properties( ${WSH_PROTOC_SOURCES} PROPERTIES GENERATED TRUE )

include_directories(
	${GLIB2_INCLUDE_DIRS}
	${PROTOBUF_INCLUDE_DIR}
	${PROTO_GEN_DIR}
	${LIBSSH_INCLUDE_DIR}
	${LIBCRANGE_INCLUDE_DIR}
	${LIBAPR_INCLUDE_DIR}
)

target_link_libraries( wsh 
	${GLIB2_LIBRARIES}
	${PROTOBUF_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
	${LIBSSH_LIBRARY}
	${LIBCRANGE_LIBRARY}
	${LIBAPR_LIBRARY}
)
