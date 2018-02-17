#!/usr/bin/env bash

BIN=./mmal_list_supported_encodings
LIST=list.txt

if ! [ -f "$BIN" ]; then
	echo "$0: error: Build $BIN by running \"make\"" >&2
	exit 1
fi
if ! [ -f "$LIST" ]; then
	echo "$0: error: $LIST not found"
	exit 1
fi

echo "# mmal_list_supported_encodings result"
echo
echo "https://github.com/Terminus-IMRC/mmal_list_supported_encodings"
echo
echo '## `vcgencmd version`'
echo '```'
vcgencmd version
echo '```'

echo
echo "## Table of contents"
< "$LIST" tr -d ' \t' | grep -v '^$' | while read COMP; do
	if echo "$COMP" | grep -q '^#'; then
		name=$(echo "$COMP" | sed 's/^#[ \t]*//')
		id=$(echo "$name" | tr '[:upper:]' '[:lower:]')
		echo "- [$name](#$id)"
		i=1
	else
		id=$(echo "$COMP" | tr -d '\.')
		echo -e "\t- [$COMP](#$id)"
	fi
done

< "$LIST" tr -d ' \t' | grep -v '^$' | while read COMP; do
	echo
	if echo "$COMP" | grep -q '^#'; then
		name=$(echo "$COMP" | sed 's/^#[ \t]*//')
		echo "## $name"
		i=1
	else
		echo "### $COMP"
		"$BIN" "$COMP" 2>/dev/null
		if [ $? -ne 0 ]; then
			echo "Failed."
		fi
	fi
done
