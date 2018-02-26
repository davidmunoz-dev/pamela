#ifndef PAMELA_H_
#define PAMELA_H_

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <libcryptsetup.h>

#define	BUFF_SIZE	2048
#define	PAM_OPEN	1
#define	PAM_CLOSE	2


void	cleanup_pamela(pam_handle_t *pamh, void *data, int error_status);
int	core_container(const char *userName, char *userPasswd, int type);
void    encrypted_container(const struct passwd *result, const char *userPasswd, const char *userName);
void    mount_container(const struct passwd *result, const char *userPasswd, const char *userName);
int	close_container(const char *userPasswd, const struct passwd *result);

#endif /* PAMELA_H_! */
