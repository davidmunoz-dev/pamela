#include "pamela.h"
#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>

void	cleanup_pamela(pam_handle_t *pamh, void *data, int error_status)
{
  (void) error_status;
  (void) pamh;
  if (data)
    {
      memset(data, 0, strlen(data));
      free(data);
    }
}

int	core_container(const char *userName, char *userPasswd, int type)
{
  char	buffer[BUFF_SIZE];
  char	dir[256];
  struct passwd *result = NULL;
  struct passwd	pwd;
  
  if (getpwnam_r(userName, &pwd, buffer, sizeof(buffer), &result) != 0
      || result == NULL || result->pw_dir == NULL)
    return (PAM_IGNORE);
  snprintf(dir, 128, "%s/secure_data-rw", result->pw_dir);
  if (type == PAM_OPEN)
    {
      if (access(dir, F_OK) == -1)
	encrypted_container(result, userPasswd, userName);
      else 
	mount_container(result, userPasswd, userName);
    }
  else
    close_container(userPasswd, result);
  return (PAM_SUCCESS);
}

void	encrypted_container(const struct passwd *result,
			    const char *userPasswd, const char *userName)
{
  char          command[256];

  snprintf(command, 256, "echo %s | sudo dd if=/dev/zero of=%s/encrypted.img bs=1 count=0 seek=1G", userPasswd, result->pw_dir);
  system(command);
  /* SHA256 */
  snprintf(command, 256, "echo -n %s | sha256sum > %s/keyfile.key", userPasswd, result->pw_dir);
  system(command);
  snprintf(command, 256, "echo %s | sudo cryptsetup luksFormat %s/encrypted.img %s/keyfile.key", userPasswd, result->pw_dir, result->pw_dir);
  system(command);
  snprintf(command, 256, "echo %s | sudo cryptsetup luksOpen -c aes %s/encrypted.img  secure_data-rw --key-file %s/keyfile.key", userPasswd, result->pw_dir, result->pw_dir);
  system(command);
  snprintf(command, 256, "echo %s | sudo mkfs.ext4 /dev/mapper/secure_data-rw", userPasswd);
  system(command);
  snprintf(command, 256, "mkdir %s/secure_data-rw", result->pw_dir);
  system(command);
  snprintf(command, 256, "echo %s | sudo mount /dev/mapper/secure_data-rw %s/secure_data-rw && chown %s:%s -R %s/secure_data-rw", userPasswd, result->pw_dir, userName, userName, result->pw_dir);
  system(command);
}

void	mount_container(const struct passwd *result,
			const char *userPasswd, const char *userName)
{
  char	command[256];
  
  snprintf(command, 256, "echo %s | sudo cryptsetup luksOpen %s/encrypted.img  secure_data-rw --key-file %s/keyfile.key", userPasswd, result->pw_dir, result->pw_dir);
  system(command);
  snprintf(command, 256, "mkdir %s/secure_data-rw", result->pw_dir);
  system(command);
  snprintf(command, 256, "echo %s | sudo mount /dev/mapper/secure_data-rw %s/secure_data-rw && chown %s:%s -R %s/secure_data-rw", userPasswd, result->pw_dir, userName, userName, result->pw_dir);
  system(command);
}

int	close_container(const char *userPasswd, const struct passwd *result)
{
  char	command[256];
  
  snprintf(command, 256, "echo %s | sudo umount %s/secure_data-rw", userPasswd, result->pw_dir);
  system(command);
  snprintf(command, 256, "echo %s | sudo cryptsetup luksClose secure_data-rw", userPasswd);
  system(command);
  printf("Closed container\n");
  return (PAM_SUCCESS);
}
