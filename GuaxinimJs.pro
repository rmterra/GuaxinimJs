#-------------------------------------------------
#
# Project created by QtCreator 2012-10-15T21:35:30
#
#-------------------------------------------------

QT       += core gui webkit network script

TARGET = GuaxinimJs
TEMPLATE = app
CONFIG   += console

unix: OBJECTS_DIR = build/.o/unix
win32: OBJECTS_DIR = build/.o/win32

AUTHOR = Ricardo Miranda Terra
EMAIL = r.mirandaterra@gmail.com
LANGUAGE = C++/Javascript
LICENSE = TDB


MOC_DIR = build/.moc

OTHER_FILES += \
	examples/*.js \
	examples/*.html \
	examples/*.css \
	examples/libs/*.js	\
	assets/*.*

SOURCES += \
	src/main.cpp \
	src/jsws.cpp \
	src/terminal.cpp \
	src/utils.cpp \
	src/requestparams.cpp \
	src/webpage.cpp \
    src/jsonparser.cpp

HEADERS  += \
	src/jsws.h \
	src/terminal.h \
	src/utils.h \
	src/const.h \
	src/requestparams.h \
	src/webpage.h \
    src/jsonparser.h
