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
	"io/ioutil"
	"os"
	"os/exec"
	"path/filepath"
)

// Update regenerates an entry in the AppArmor cache on disk
func Update(path string) error {
	cmd := exec.Command("apparmor_parser",
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
	fs, err := ioutil.ReadDir(dir) // get all the files in the cache
	if err != nil {
		return err
	}

	for _, f := range fs { // for each file
		if f.IsDir() { // skip if its a directory
			continue
		}

		e := Entry{ // Create a new Entry
			Mod:  f.ModTime(), // Add the mod time of the file
			Path: dir,         // Add the directory path
		}

		name := f.Name()
		m[name] = append(m[name], e) // Add to our ProfMap

		// check if update is needed
		update := true
		if entries := cache[name]; len(entries) > 0 {
			for _, cached := range entries {
				if !cached.Mod.Before(e.Mod) {
					update = false
				}
			}
		}

		if update {
			if err = Update(filepath.Join(dir, name)); err != nil {
				return err
			}
		}
	}
	return nil
}

// UpdateAll calls UpdatePath for all profile directories that were configured
func UpdateAll(dirs []string, cached ProfMap) (ProfMap, error) {
	profs := make(ProfMap)
	for _, dir := range dirs { // update every directory, one by one
		if err := profs.UpdatePath(dir, cached); err != nil {
			return profs, err
		}
	}
	return profs, nil
}
