QT       += core gui webkit network script

TARGET = GuaxinimJs
TEMPLATE = app
CONFIG   += console

unix: OBJECTS_DIR = build/.o/unix
win32: OBJECTS_DIR = build/.o/win32

AUTHOR = Ricardo Miranda Terra
EMAIL = r.mirandaterra@gmail.com
LANGUAGE = C++/Javascript
LICENSE = BSD LICENSE

#FOR WINDOWS REMOVE THIS PATH AND INCLUDE YOUR PROJECT DIRECTORY
win32: INCLUDEPATH += D:\WinDevelop\QT\rmterra-GuaxinimJs

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
	src/jsonparser.cpp \
	src/webrequest.cpp

HEADERS  += \
	src/jsws.h \
	src/terminal.h \
	src/utils.h \
	src/const.h \
	src/requestparams.h \
	src/webpage.h \
	src/jsonparser.h \
	src/webrequest.h
