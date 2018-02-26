#include "pamela.h"
#include "syslog.h"
#include <string.h>

PAM_EXTERN int  pam_sm_authenticate(pam_handle_t *pamh,   int flags,
				    int	argc, const char **argv)
{
  const char	*userName = NULL;
  char		*userPasswd;
  struct passwd *pwd;
  int	error;
  (void)pamh;
  (void)argc;
  (void)argv;
  (void)flags;
  
  if ((error = pam_get_user(pamh, &userName, "Username : ")) != PAM_SUCCESS)
    {
      syslog(LOG_ERR, "Cannot determine user name");
      return (error);
    }
  if ((pwd = getpwnam(userName)) == NULL)
    {
      syslog(LOG_ERR, "Password not found for this user");
      return (PAM_USER_UNKNOWN);
    }
  if ((error = pam_get_item(pamh, PAM_AUTHTOK, (const void **)&userPasswd)) != PAM_SUCCESS)
    {
      syslog(LOG_ERR, "Error getting the token authentification");
      return (error);
    }
  if ((error = pam_set_data(pamh, "pam_module_pass", strdup(userPasswd), &cleanup_pamela)) != PAM_SUCCESS)
    {
      syslog(LOG_ERR, "Error saving the password on pam_module_pass");
      return (error);
    }
  write(1, "Authentification : OK\n", 22);
  return (PAM_SUCCESS);
}

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags,
			      int argc, const char **argv)
{
  (void)argc;
  (void)argv;
  (void)pamh;
  (void)flags;
  return (PAM_SUCCESS);
}

PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags,
		     int argc, const char **argv)
{
  (void)argc;
  (void)argv;
  (void)pamh;
  (void)flags;
  return (PAM_SUCCESS);
}

PAM_EXTERN int pam_sm_open_session(pam_handle_t *pamh, int flags,			
			int argc, const char **argv)
{
  const char	*userName = NULL;
  char		*userPasswd;
  (void)argc;
  (void)argv;
  (void)flags;


  if (pam_get_data(pamh, "pam_module_pass", (const void **)&userPasswd) != PAM_SUCCESS)
      return (PAM_SYSTEM_ERR);
  if (pam_get_user(pamh, &userName, "Username : ") != PAM_SUCCESS)
    {
      syslog(LOG_ERR, "Cannot determine user name");
      return (PAM_USER_UNKNOWN);
    }
  if (core_container(userName, userPasswd, PAM_OPEN) != PAM_SUCCESS)
    return (PAM_IGNORE);
  printf("[PAM]: Container secure_data-rw created\n");
  return (PAM_SUCCESS);
}

PAM_EXTERN int pam_sm_close_session(pam_handle_t *pamh, int flags,			 
				    int argc, const char **argv)
{
  const char	*userName = NULL;
  char		*userPasswd;
  (void)argc;
  (void)argv;
  (void)flags;
  
  if (pam_get_data(pamh, "pam_module_pass", (const void **)&userPasswd) != PAM_SUCCESS)
    return (PAM_SYSTEM_ERR);
  if (pam_get_user(pamh, &userName, "Username : ") != PAM_SUCCESS)
    {
      syslog(LOG_ERR, "Cannot determine user name");
      return (PAM_USER_UNKNOWN);
    }
  if (core_container(userName, userPasswd, PAM_CLOSE) != PAM_SUCCESS)
    return (PAM_IGNORE);
  return (PAM_SUCCESS);
}

PAM_EXTERN int pam_sm_chauthtok(pam_handle_t *pamh, int flags,
				int argc, const char **argv)
{
  (void)argc;
  (void)argv;
  (void)flags;
  (void)pamh;
  return (PAM_SUCCESS);
}
