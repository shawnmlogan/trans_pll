#!/bin/bash

if test -d $HOME/bin; then
	if test -L $HOME/bin/trans_pll; then
		# echo "File trans_pll exists...deleting file to replace it with new version."
		rm $HOME/bin/trans_pll
	fi
	ln -s $PWD/trans_pll $HOME/bin/trans_pll
	# echo "File trans_pll created..."
	# ls -l $HOME/bin/trans_pll
fi
