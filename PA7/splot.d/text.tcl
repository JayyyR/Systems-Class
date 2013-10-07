
canvas .c -width 300 -height 300 -bg white
pack .c

.c create line 0 100 300 100 -width 4
.c create line 0 200 300 200 -fill red
.c create line 100 0 100 300
.c create line 200 0 200 300

.c create text 0 0 -anchor nw -text "Northwest Anchor"
.c create text 0 100 -anchor w -text "West Anchor" \
  -tags westtext
.c create text 150 150 -anchor c -text "Centered"
.c create text 150 300 -anchor s -text "South" -fill red \
  -font "-adobe-helvetica-medium-o-normal--18" \
  -tags southtext

.c bind westtext <Button-1> {\
	.c itemconfigure westtext -text "Foo"}

set scolor navyblue

.c bind southtext <Button-1> {\
	.c itemconfigure southtext -fill $scolor; \
	if {$scolor == "navyblue"} {set scolor red} else {set scolor navyblue}}

