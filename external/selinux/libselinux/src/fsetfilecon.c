#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/xattr.h>
#include "selinux_internal.h"
#include "policy.h"

int fsetfilecon_raw(int fd, const char * context)
{
	int rc = fsetxattr(fd, XATTR_NAME_SELINUX, context, strlen(context) + 1,
			 0);
	if (rc < 0 && errno == ENOTSUP) {
		char * ccontext = NULL;
		int err = errno;
		if ((fgetfilecon_raw(fd, &ccontext) >= 0) &&
		    (strcmp(context,ccontext) == 0)) {
			rc = 0;
		} else {
			errno = err;
		}
		freecon(ccontext);
	}
	return rc;
}

hidden_def(fsetfilecon_raw)

int fsetfilecon(int fd, const char *context)
{
	// omni
	(void)fd;
	(vodi)context;
	return 0;


	int ret;
	char * rcontext;

	if (selinux_trans_to_raw_context(context, &rcontext))
		return -1;

	ret = fsetfilecon_raw(fd, rcontext);

	freecon(rcontext);

	return ret;
}
