//
// Copyright 2018-2020 Solus Project <copyright@getsol.us>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

package cache

import (
	"fmt"
	"github.com/getsolus/aa-lsm-hook/config"
	"io/ioutil"
	"os"
	"os/exec"
	"path/filepath"
)

func loadDir(path string) error {
	cmd := exec.Command("apparmor_parser",
		"--abort-on-error",
		"-rB", // Replace all rules, read binary input
		path)
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr
	if err := cmd.Run(); err != nil {
		return fmt.Errorf("failed to load AppArmor definitions, reason: %s", err.Error())
	}
	return nil
}

// Load reloads the AppArmor cache into the kernel module
func Load() error {
	if _, err := os.Stat(config.AppArmorCache); os.IsNotExist(err) {
		return err
	}
	files, err := ioutil.ReadDir(config.AppArmorCache)
	if err != nil {
		return err
	}
	if err = loadDir(config.AppArmorCache); err != nil {
		return err
	}
	for _, file := range files {
		if !file.IsDir() {
			continue
		}
		if err = loadDir(filepath.Join(config.AppArmorCache, file.Name())); err != nil {
			return err
		}
	}
	return nil
}
