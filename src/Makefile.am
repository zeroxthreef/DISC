lib_LIBRARIES = libDisC.a

AM_CFLAGS = -g

AM_LDFLAGS = -lssl -lcrypto -ljansson

libDisC_adir = $(includedir)/DisC

libDisC_a_HEADERS = \
                    DisC.h \
                    DisC_types.h \
                    DisC_errors.h \
                    DisC_util.h \
                    DisC_rest.h \
                    DisC_object.h \
                    DisC_gateway.h

libDisC_a_SOURCES = \
                    $(libDisC_a_HEADERS) \
                    DisC.c \
                    DisC_errors.c \
                    DisC_util.c \
                    DisC_rest.c \
                    DisC_object.c \
                    DisC_gateway.c
