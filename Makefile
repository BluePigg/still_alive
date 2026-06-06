CC = g++
TARGET = main
SRCS = main.cpp

$(TARGET): ${SRCS}
	${CC} -o ${TARGET} ${SRCS} -lpthread -lm -ldl
