#!/bin/bash

PANES=`tmux list-panes -F \#P`
if [ $PANES==0 ]; then
    tmux split-window -h './server'
    tmux select-pane -t 0
    tmux send-keys -t 0 './client' 'Enter'
else
    tmux send-keys -t 1 './server' 'Enter'
    tmux select-pane -t 0
    tmux send-keys -t 0 './client' 'Enter'
fi
