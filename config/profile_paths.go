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

package config

import (
	"bufio"
	"errors"
	"io"
	"os"
)

// ConfigFiles is a list of config files to check for, in order of ascending priority
var ConfigFiles = []string{
	"/usr/share/defaults/etc/aa-lsm-hook.conf",
	"/etc/aa-lsm-hook.conf",
}

func parseConfigFile(name string) (dirs []string, err error) {
	_, e := os.Stat(name)
	if e != nil {
		if os.IsNotExist(e) { // skip missing files
			return
		}
		err = e
		return
	}
	f, err := os.Open(name) // Open the File
	if err != nil {
		return
	}
	r := bufio.NewReader(f)
	// Read each line
	var raw []byte
	for {
		raw, _, err = r.ReadLine()
		// Check for I/O error or last line
		if err != nil {
			break
		}
		// skip check for empty lines
		if len(raw) == 0 {
			continue
		}
		path := string(raw)
		if _, e := os.Stat(path); e == nil {
			dirs = append(dirs, path) // Add to list if the directory exists
		}
	}
	if err == io.EOF {
		err = nil
	}
	// Close files
	_ = f.Close()
	return
}

// ErrNoProfileDirs is returned if no profile dirs have been configured
var ErrNoProfileDirs = errors.New("failed to find any configured profile paths")

// ProfileDirs retrieves a list of directories to search for profiles, listed one per line in each of the ConfigFiles
func ProfileDirs() (dirs []string, err error) {
	var newDirs []string
	for _, file := range ConfigFiles {
		newDirs, err = parseConfigFile(file)
		if err != nil {
			return
		}
		dirs = append(dirs, newDirs...)
	}
	if len(dirs) == 0 {
		err = ErrNoProfileDirs
		return
	}
	return
}
