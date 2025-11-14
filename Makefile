ifneq ("$(wildcard /usr/share/sharness/sharness.sh)","")
SHARNESS := /usr/share/sharness
endif

ifneq ("$(wildcard /usr/local/share/sharness/sharness.sh)","")
SHARNESS := /usr/local/share/sharness
endif

ifneq ("$(wildcard $HOME/.local/share/sharness/sharness.sh)","")
SHARNESS := $HOME/.local/share/sharness
endif

CFLAGS ?= -O0 -g3 -Wall
#CFLAGS ?= -O0 -g3 -Wall -fsanitize=address,undefined -fno-sanitize-recover
#CFLAGS ?= -O2 -Wall

all: sp800232-rit_driver

sp800232-rit_driver: sp800232-rit_driver.c sp800232-rit.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test/LWC_AEAD_KAT_128_128.txt:
	cd test && wget https://raw.githubusercontent.com/ascon/ascon-c/refs/heads/main/crypto_aead/asconaead128/LWC_AEAD_KAT_128_128.txt

test/aead128_auto.t: test/parse.py test/LWC_AEAD_KAT_128_128.txt
	python3 $< < $(word 2,$^) > $@

test: sp800232-rit_driver test/aead128_manual.t test/aead128_auto.t
	SHARNESS_TEST_SRCDIR=$(SHARNESS) prove -v test/aead128_manual.t
	SHARNESS_TEST_SRCDIR=$(SHARNESS) prove -v test/aead128_auto.t

format:
	clang-format --style=WebKit -i *.c *.h

.PHONY: clean test format
clean:
	rm -f sp800232-rit_driver
