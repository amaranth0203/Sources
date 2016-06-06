NAME = test14
OBJS = $(NAME).obj

LINK_FLAG = /subsystem:console
ML_FLAG = /c /coff

$(NAME).exe: $(OBJS)
	Link $(LINK_FLAG) $(OBJS)
%.obj : %.asm
	ml $(ML_FLAG) $<

clean:
	rm *.obj
	rm *.exe