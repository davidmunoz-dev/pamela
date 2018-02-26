TARGET	 = pamela.so

CC	 = gcc

SRC      = pam_module.c		\
	   container.c

CFLAGS   += -fPIC -fno-stack-protector -Iinclude

CFLAGS	 += -W -Wall -Werror

LDFLAGS	 += -lcryptsetup

SRCDIR	 = src
OBJDIR	 = obj

SOURCES  := $(addprefix src/, $(SRC))

OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

rm       = rm -rf
mkdir    = mkdir -p

$(TARGET): $(OBJECTS)
	@$(CC) $(CFLAGS) -o $@ $(OBJECTS) -shared -rdynamic $(CFLAGS) $(LDFLAGS)
	@echo "\033[1;32m"$(TARGET)" compiled !\033[0m"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(mkdir) $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@

all     : $(TARGET)

clean   :
	@$(rm) $(OBJDIR)
	@echo "\033[0;31mBinary files deleted\033[0m"

fclean  : clean
	@$(rm) $(TARGET)
	@echo "\033[0;31mBinary and executable files are deleted\033[0m"

re      : fclean all

install:
ifneq ("$(wildcard /lib/security/$(TARGET))", "")
	@printf "\033[0;31m[PAM] already installed\033[0m"
else
	apt-get install -y gcc cryptsetup libcryptsetup-dev libpam0g-dev
	make
	@$(mkdir) /lib/security
	@cp $(TARGET) /lib/security/$(TARGET)
	@echo "auth	optional	pamela.so" >> /etc/pam.d/common-auth
	@echo "session	optional	pamela.so" >> /etc/pam.d/common-session
	@echo "account	optional	pamela.so" >> /etc/pam.d/common-account
	@echo "password	optional	pamela.so" >> /etc/pam.d/common-password
	@echo "\033[1;32m[PAM] installed successfully\033[0m"
endif

uninstall:
ifeq ("$(wildcard /lib/security)", "")
	@printf "\033[0;31m[PAM] not installed\033[0m"
else
	make fclean
	@sed '26d;27d' /etc/pam.d/common-auth > /tmp/common-auth && mv /tmp/common-auth /etc/pam.d/common-auth
	@sed '26d;27d' /etc/pam.d/common-session > /tmp/common-session && mv /tmp/common-session /etc/pam.d/common-session
	@sed '26d;27d' /etc/pam.d/common-account > /tmp/common-account && mv /tmp/common-account /etc/pam.d/common-account
	@sed '35d' /etc/pam.d/common-password > /tmp/common-password && mv /tmp/common-password /etc/pam.d/common-password
	@$(rm) /lib/security
	@echo "\033[1;32m[PAM] uninstalled\033[0m"
endif

test:

check:
ifeq ("$(wildcard /lib/security)", "")
	@echo "\033[0;31m[PAM] not installed\033[0m"
else
	@echo "\033[1;32m[PAM] installed\033[0m"
endif

.PHONY: all clean fclean re install uninstall test check
