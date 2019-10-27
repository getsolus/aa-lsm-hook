//
// Copyright 2018-2019 Solus Project <copyright@getsol.us>
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

package profiles

import (
	"fmt"
	"github.com/getsolus/aa-lsm-hook/config"
	"io/ioutil"
	"os"
	"os/exec"
	"path/filepath"
)

// Update regenerates an entry in the AppArmor cache on disk
func Update(path string) error {
	cmd := exec.Command("apparmor_parser",
		"-WQTL", // Replace all rules, reaf binary input
		config.AppArmorCache,
		path,
		"--abort-on-error")
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr
	if err := cmd.Run(); err != nil {
		return fmt.Errorf("failed to update AppArmor profile, reason: %s", err.Error())
	}
	return nil
}

// UpdatePath updates all of the profiles in a directory if their cache is older than the profile or doesn't exist
func (m ProfMap) UpdatePath(dir string, cache ProfMap) error {
	// get all the files in the cache
	fs, err := ioutil.ReadDir(dir)
	if err != nil {
		return err
	}
	// for each file
	for _, f := range fs {
		// skip if its a directory
		if f.IsDir() {
			continue
		}
		// track that it was processed
		e := Entry{
			Name: dir,
			Mod:  f.ModTime(),
		}
		m[f.Name()] = append(m[f.Name()], e)
		// check if update is needed
		update := true
		if entries := cache[f.Name()]; len(entries) > 0 {
			for _, cached := range entries {
				if !cached.Mod.Before(e.Mod) {
					update = false
				}
			}
		}
		// update as needed
		if update {
			err = Update(filepath.Join(dir, f.Name()))
			if err != nil {
				return err
			}
		}
	}
	return nil
}

// UpdateAll calls UpdatePath for all profile directories that were configured
func UpdateAll(dirs []string, cached ProfMap) (ProfMap, error) {
	profs := make(ProfMap)
	// update every directory, one by one
	for _, dir := range dirs {
		if err := profs.UpdatePath(dir, cached); err != nil {
			return profs, err
		}
	}
	return profs, nil
}
