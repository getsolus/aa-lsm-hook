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
	"io/ioutil"
	"path/filepath"
	"time"
)

// Entry is an item in a ProfMap representing a profile
type Entry struct {
	// Name is the location where this profile is stored on disk
	Name string
	// Mod is the last time the profile was modified
	Mod time.Time
}

// ProfMap relates multiple profiles of the same name to their different locations
type ProfMap map[string][]Entry

// AddProfiles finds all of the profiles in a current directory and one level deeper
func (m ProfMap) AddProfiles(dir string) error {
	// get all the files in the current directory
	fs, err := ioutil.ReadDir(dir)
	if err != nil {
		return err
	}
	// for each file
	for _, f := range fs {
		// skip feature files
		if f.Name() == ".features" {
			continue
		}
		// traverse to the next level
		if f.IsDir() {
			err = m.AddProfiles(filepath.Join(dir, f.Name()))
			if err != nil {
				return err
			}
			continue
		}
		// store an entry for this file
		e := Entry{
			Name: dir,
			Mod:  f.ModTime(),
		}
		m[f.Name()] = append(m[f.Name()], e)
	}
	return nil
}
