NAME=Auducam_Mega
CC=g++
CFLAGS=-Wall -pthread `pkg-config --cflags --libs opencv`
 
ODIR=obj
SDIR=src

_DEPS=Arducam.h bsp_spi.h Platform.h raspberryHal.h 
DEPS=$(patsubst %,$(SDIR)/%,$(_DEPS))

_OBJ=main.o Arducam.o bsp_spi.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: src/%.cpp $(DEPS)
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS)
$(ODIR)/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(NAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY:	run

run:	$(NAME)
	sudo ./$(NAME)

.PHONY:	clean
clean:	
	rm -f $(NAME) $(ODIR)/*.o