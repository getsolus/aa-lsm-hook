package cache

import (
	"errors"
	"fmt"
	"io/fs"
	"os"

	"github.com/getsolus/aa-lsm-hook/config"
)

// Init initializes the cache directory.
func Init() error {
	if _, err := os.Stat(config.AppArmorCache); err != nil && errors.Is(err, fs.ErrNotExist) {
		if err := os.Mkdir(config.AppArmorCache, config.AppArmorCachePermissions); err != nil {
			return fmt.Errorf("failed to initialize apparmor cache: %w", err)
		}
	}

	return nil
}
