# splot.tcl

# Called whenever we replot the points
proc replot val { puts stdout " [.sf.timescroll get] "
        
        flush stdout
}

# Create canvas widget
canvas .c -width 600 -height 600 -bg white
pack .c -side top

.c create text 600 0 -anchor ne -text "Avg Lifetime: 0.00" -tags avgTime

#change beetle lifetime
.c bind beetleNum <Button-1> { puts stdout "lifetime"}

# 
# frame .ef
# pack .ef -expand 1 -fill x -side top -anchor n -before .c 
# 
# button .ef.b4 -text "Beetle Number:" -command {puts stdout "lifetime"}
# pack .ef.b4 -side top -expand 0   -anchor nw 

# Frame for holding buttons
frame .bf
pack  .bf -expand 1 -fill x

# Exit button
button .b1 -text "Exit" -command {exit}

# Reset button
button .b2 -text "Silent" -command  {puts stdout "silent"}

# Pack buttons into frame
pack .b1 .b2 -side left \
    -expand 1 -fill x

# Frame to hold scrollbars
frame .sf
 pack  .sf -expand 1 -fill x -side bottom -after .bf

# Scrollbars for rotating view.  Call replot whenever
# we move them.
scale .sf.timescroll -label "Time Scaling" -length 500 \
-from .1 -to 5 -orient horiz -showvalue 1 -resolution .1

button .sf.b3 -text "Time\nScale" -command {puts stdout "scale\n [.sf.timescroll get]"}

pack .sf.b3 -side left -fill x -expand 1
# 
# Pack them into the frame
pack .sf.timescroll -side top
