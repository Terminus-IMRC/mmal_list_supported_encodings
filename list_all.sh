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
echo '```'
vcgencmd version
echo '```'

< "$LIST" tr -d ' \t' | grep -v '^#' | grep -v '^$' | while read COMP; do
	echo
	echo "## $COMP"
	"$BIN" "$COMP" 2>/dev/null
	if [ $? -ne 0 ]; then
		echo "Failed."
	fi
done
