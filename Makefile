TARGET = v4l2_capture

SRCS = main.c \
       v4l2_common.c \
       querycap.c \
       enum_fmt.c \
       enum_framesizes.c \
       s_fmt.c \
       reqbufs.c \
       querybuf.c \
       qbuf.c \
       dqbuf.c \
       streamon.c \
       streamoff.c \
       memory.c \
       frame.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	gcc -o $(TARGET) $(OBJS)

%.o: %.c
	gcc -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)