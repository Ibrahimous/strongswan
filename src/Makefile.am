SUBDIRS = . include

if USE_LIBSTRONGSWAN
  SUBDIRS += libstrongswan
endif

if USE_LIBHYDRA
  SUBDIRS += libhydra
endif

if USE_LIBIPSEC
  SUBDIRS += libipsec
endif

if USE_SIMAKA
  SUBDIRS += libsimaka
endif

if USE_TLS
  SUBDIRS += libtls
endif

if USE_RADIUS
  SUBDIRS += libradius
endif

if USE_LIBTNCIF
  SUBDIRS += libtncif
endif

if USE_LIBTNCCS
  SUBDIRS += libtnccs
endif

if USE_LIBPTTLS
  SUBDIRS += libpttls
endif

if USE_IMCV
  SUBDIRS += libimcv
endif

if USE_PTS
  SUBDIRS += libpts
endif

if USE_LIBCHARON
  SUBDIRS += libcharon
endif

if USE_FILE_CONFIG
  SUBDIRS += starter
endif

if USE_IPSEC_SCRIPT
  SUBDIRS += ipsec _copyright
endif

if USE_CHARON
  SUBDIRS += charon
endif

if USE_NM
  SUBDIRS += charon-nm
endif

if USE_STROKE
  SUBDIRS += stroke
endif

if USE_UPDOWN
  SUBDIRS += _updown _updown_espmark
endif

if USE_TOOLS
  SUBDIRS += scepclient pki
endif

if USE_SWANCTL
  SUBDIRS += swanctl
endif

if USE_CONFTEST
  SUBDIRS += conftest
endif

if USE_DUMM
  SUBDIRS += dumm
endif

if USE_FAST
  SUBDIRS += libfast
endif

if USE_MANAGER
  SUBDIRS += manager
endif

if USE_MEDSRV
  SUBDIRS += medsrv
endif

if USE_ATTR_SQL
  SUBDIRS += pool
else
if USE_SQL
  SUBDIRS += pool
endif
endif

if USE_TKM
  SUBDIRS += charon-tkm
endif

if USE_CMD
  SUBDIRS += charon-cmd
endif

if USE_LIBPTTLS
  SUBDIRS += pt-tls-client
endif

if USE_INTEGRITY_TEST
  SUBDIRS += checksum
endif

if USE_AIKGEN
  SUBDIRS += aikgen
endif

#GCOV ADD
gcov-prehook: gcov-prehook-recursive
gcov-prehook-recursive:
	for lib_dir in $(SUBDIRS); do \
	  if [ $$lib_dir != "." ]; then \
	    $(MAKE) gcov-prehook -C $$lib_dir; \
	  fi; \
	done

# Create on every host executing strongswan directories where we'll rapatriate coverage results
gcov-posthook: gcov-posthook-recursive
gcov-posthook-recursive:
	for lib_dir in $(SUBDIRS); do \
	  if [ $$lib_dir != "." ]; then \
	    $(MAKE) gcov-posthook -C $$lib_dir; \
	  fi; \
	done

# Host VM from which we pick results
HOST=$(HOST)
# Host's address connection
HOSTLOGIN=$(HOSTLOGIN)
# Tests directory
TESTRESULTSDIR=$(TESTRESULTSDIR)

# Create in the user-space directories where we'll rapatriate coverage results 
create-gcov-results-directories:
	for lib_dir in $(SUBDIRS); do \
	  if [ $$lib_dir != "." ]; then \
	    mkdir -p $$TESTRESULTSDIR/cov/$$HOST/html/$$lib_dir; \
	    cd $$lib_dir; \
	    if [ -d "plugins" ]; then \
	      cd "plugins"; \
	      for plug_dir in `ls -d */ | xargs -n1 basename`; do \
	      	mkdir -p $$TESTRESULTSDIR/cov/$$HOST/html/$$lib_dir/plugins/$$plug_dir; \
	      done; \
	      cd ..; \
	    fi; \
	    cd ..; \
	  fi; \
	done

# Update web site in order to integrate all results for A GIVEN HOST
test-gcov-final:
	mkdir -p $$TESTRESULTSDIR/html/general
	lcov --directory $(CURDIR) \
		--capture \
		--output-file  $$TESTRESULTSDIR/general.info \
		&& genhtml -o $$TESTRESULTSDIR/html/general $$TESTRESULTSDIR/general.info
#END of GCOV ADD
