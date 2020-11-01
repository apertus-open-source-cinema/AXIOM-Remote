#!/bin/bash

OUT="${1:-out.h}"
XBM="${2:-/dev/stdin}"

TEMPLATE="icon_template.h"

eval $(cat "$XBM" | tr '\n\r' '\n' | sed '
	s/^#define.*_width /WIDTH=/;
	s/^#define.*_height /HEIGHT=/;
	s/^static char.*{/DATA="/;
	s/\s*};/"/')

BASE="${OUT%.h}"
DATA=$(echo "$DATA" | sed 's/^\s*//;s/,\s*$//')

cat "$TEMPLATE" | sed "
	s/%ICON_TEMPLATE_H%/\U${BASE}_H\E/g;
	s/%ICON_NAME%/${BASE}/g;
	s/%WIDTH%/${WIDTH}/g;
	s/%HEIGHT%/${HEIGHT}/g;
	s/%IMAGE_DATA%/${DATA}/" >"${OUT}"
