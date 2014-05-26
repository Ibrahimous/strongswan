#!/bin/bash
#transform Makefiles in order to obtain a code coverage enabled testing environment
#Launch it from scripts directory, where plugins_ajout_gcov-hooks_dans_Makefile_am and lib_... stay as well

cd ../src
#We are in src now: get the lib list, dude
SUBDIRS=`ls -d */ | xargs -n1 basename`
#Enter in each lib, modify Makefile.am, and if it exists, do the same with each library's plugin
for subdir in $SUBDIRS
do
	cd $subdir
	cat ../../scripts/lib_ajout_gcov-hooks_dans_Makefile_am >> Makefile.am
	if [ -d "plugins" ]; then
	  cd "plugins"
	  PLUGINS=`ls -d */ | xargs -n1 basename`
	  for plug_dir in $PLUGINS
		do
		  cat ../../../scripts/plugins_ajout_gcov-hooks_dans_Makefile_am >> ./$plug_dir/Makefile.am
		done
	  cd ..
	fi
	cd ..
done
