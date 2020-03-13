PKGNAME  = aa-lsm-hook
DESTDIR ?=
PREFIX  ?= /usr
BINDIR   = $(PREFIX)/sbin
LIBDIR ?= $(PREFIX)/lib
SYSCONFDIR ?= /usr/share/defaults/etc

GOPROJROOT  = $(GOSRC)/$(PROJREPO)

GOBIN       = ~/go/bin
GOLDFLAGS   = -ldflags "-s -w"
GOTAGS      = -tags "osusergo"
GOCC        = go
GOFMT       = $(GOCC) fmt -x
GOGET       = $(GOCC) get $(GOLDFLAGS)
GOBUILD     = $(GOCC) build -v $(GOLDFLAGS) $(GOTAGS)
GOTEST      = $(GOCC) test
GOVET       = $(GOCC) vet
GOINSTALL   = $(GOCC) install $(GOLDFLAGS)

include Makefile.waterlog

GOLINT    = $(GOBIN)/golint -set_exit_status

all: build

build: setup-deps
	@$(call stage,BUILD)
	@$(GOBUILD)
	@$(call pass,BUILD)

test: build
	@$(call stage,TEST)
	@$(GOTEST) ./...
	@$(call pass,TEST)

validate: setup-deps
	@$(call stage,FORMAT)
	@$(GOFMT) ./...
	@$(call pass,FORMAT)
	@$(call stage,VET)
	@$(call task,Running 'go vet'...)
	@$(GOVET) ./...
	@$(call pass,VET)
	@$(call stage,LINT)
	@$(call task,Running 'golint'...)
	@$(GOLINT) `go list ./... | grep -v vendor`
	@$(call pass,LINT)

setup-deps:
	@$(call stage,DEPS)
	@if [ ! -e $(GOBIN)/golint ]; then \
	    $(call task,Installing golint...); \
	    $(GOGET) golang.org/x/lint/golint; \
	fi

install:
	@$(call stage,INSTALL)
	install -Dm 00755 $(PKGNAME) $(DESTDIR)$(BINDIR)/$(PKGNAME)
	install -Dm 00644 data/aa-lsm-hook.conf $(DESTDIR)$(SYSCONFDIR)/aa-lsm-hook.conf
	install -Dm 00644 data/aa-lsm-hook.service $(DESTDIR)$(LIBDIR)/systemd/system/aa-lsm-hook.service
	@$(call pass,INSTALL)

uninstall:
	@$(call stage,UNINSTALL)
	rm -f $(DESTDIR)$(BINDIR)/$(PKGNAME)
	@$(call pass,UNINSTALL)

clean:
	@$(call stage,CLEAN)
	@$(call task,Removing executable...)
	@rm $(PKGNAME)
	@$(call pass,CLEAN)
