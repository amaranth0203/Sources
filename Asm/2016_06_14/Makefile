NAME = Dialog
OBJS = $(NAME).obj
RES  = $(NAME).res

LINK_FLAG = /subsystem:windows
ML_FLAG = /c /coff

$(NAME).exe: $(OBJS) $(RES)
	Link $(LINK_FLAG) $(OBJS) $(RES)

%.obj:%.asm
	ml $(ML_FLAG) $<
%.res:%.rc
	rc $<

clean:
	del *.obj
	del *.res
	del *.exe