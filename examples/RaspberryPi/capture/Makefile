Target := Arducam_Mega
CXX := gcc
Source := $(wildcard *.c)
DIR := $(notdir $(Source)) 
OBJ := $(patsubst %.c,%.o,$(DIR))
LIBS=-lgpiod

$(Target):$(OBJ)
	$(CXX) $^ $(LIBS) -o $@


.PHONY:clean

clean:
	rm -rf *.jpg
	rm -rf *.o
	rm -rf $(Target)
