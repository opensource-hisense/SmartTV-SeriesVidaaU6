INCLUDE(FindPkgConfig OPTIONAL)

IF(PKG_CONFIG_FOUND)
	INCLUDE(FindPkgConfig)
	PKG_CHECK_MODULES(LIBSRTP2 srtp2)
ENDIF(PKG_CONFIG_FOUND)

IF(NOT LIBSRTP2_FOUND)
	FIND_PATH(LIBSRTP2_INCLUDE_DIR srtp2/srtp.h
		/usr/local/include
		/usr/include
	)

	FIND_LIBRARY(LIBSRTP2_LIBRARY_DIR
		NAMES srtp2
		PATH
		/usr/local/lib
		/usr/local/lib64
		/usr/lib
		/usr/lib64
	)
	MESSAGE(STATUS "Found LIBSRTP2: ${LIBSRTP2_LIBRARY_DIR} ${LIBSRTP2_INCLUDE_DIR}")
ENDIF(NOT LIBSRTP2_FOUND)

IF(LIBSRTP2_LIBRARY_DIR AND LIBSRTP2_INCLUDE_DIR)
	SET(LIBSRTP2_FOUND TRUE)
ENDIF(LIBSRTP2_LIBRARY_DIR AND LIBSRTP2_INCLUDE_DIR)

IF(NOT LIBSRTP2_FOUND)
	IF(LIBSRTP2_FIND_REQUIRED)
		MESSAGE(FATAL_ERROR "Could not find LIBSRTP2")
	ENDIF(LIBSRTP2_FIND_REQUIRED)
ENDIF(NOT LIBSRTP2_FOUND)
