PROJ := mmal_list_supported_encodings

all: $(PROJ)

CFLAGS := -I/opt/vc/include -pipe -Wall -Wextra -g -O2
LDLIBS := -L/opt/vc/lib

$(PROJ): $(PROJ).o
$(PROJ): LDLIBS += -lbcm_host -lvcos -lmmal_core -lmmal_util -lmmal_vc_client

.PHONY: clean
clean:
	$(RM) $(PROJ)
	$(RM) $(PROJ).o
