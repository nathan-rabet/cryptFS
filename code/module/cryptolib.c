#include <linux/module.h>
#include "fs_info.h"

static __init int fs_load(void)
{
	pr_debug(CRYPT_FS_NAME##": Loading...\n");

	int ret = register_filesystem(&fs_type);
	if (ret != 0) {
		pr_err(CRYPT_FS_NAME ": cannot register filesystem\n");
		return ret;
	}

	pr_debug(CRYPT_FS_NAME ": Loaded successfully...\n");
	return 0;
}

static __exit void fs_unload(void)
{
	pr_debug(CRYPT_FS_NAME ": Unloading...\n");
	int ret = unregister_filesystem(&fs_type);
	if (ret != 0)
		pr_err("saltfs: cannot unregister filesystem\n");
	pr_debug(CRYPT_FS_NAME ": Unloaded successfully...\n");
}

module_init(fs_load);
module_exit(fs_unload);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("CryptoFS");
MODULE_AUTHOR("RABET Nathan");
MODULE_AUTHOR("MOHAMMED BEN ALI Sabir");
MODULE_AUTHOR("REKIK Youness");
